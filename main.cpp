#include <iostream>
#include "BigInt.h"
#include "IntegerMod.h"
#include "Logger.h"
#include "Field.h"
constexpr char const* file = "config.json";
constexpr char const* fileEnc = "configEnc.json";
constexpr char const* fileDec = "configDec.json";


struct config
{
    std::string p;
    std::string q;
    std::string file;
    std::string mode;
    std::string LogLevel;
};

struct configEnc
{
    std::string e;
    std::string n;
};

struct configDec
{
    std::string d;
    std::string n;
};


std::string find(std::string const& str, std::string const& key)
{
    auto k = str.find(key);
    if(k == std::string::npos)
    {
        throw error::ExeptionBase<error::ErrorList::InputError>("Lose mandatory field " + key);
    }
    k = str.find(':', k + 1);
    if(k == std::string::npos)
    {
        throw error::ExeptionBase<error::ErrorList::InputError>("Lose mandatory value of field " + key);
    }
    auto start = str.find("\"",k + 1);
    auto end   = str.find( "\"", start + 1);
    if(end == str.size())
        end  = str.find( "\n", start + 1);

    return str.substr(start + 1, end - start - 1);
};
config parse(std::string const& str)
{
    config cfg;
    cfg.p = find(str,"P");
    cfg.q = find(str,"Q");
    cfg.file = find(str,"File");
    cfg.LogLevel = find(str,"LogLevel");
    cfg.mode    = find(str,"Mode");
    return cfg;
}
configEnc parseEnc(std::string const& str)
{
    configEnc cfg;
    cfg.e = find(str,"E");
    cfg.n = find(str,"N");
    return cfg;
}
configDec parseDec(std::string const& str)
{
    configDec cfg;
    cfg.d = find(str,"D");
    cfg.n = find(str,"N");
    return cfg;
}

enum Mode : size_t
{
      generatorKey
    , encrypt
    , decrypt
};

int main()
{
    Logger::setLevel(Log::Level::debug);
    bool opened = true;
    try {
        Logger::start();
    }
    catch (error::Exeption &exp) {
        std::cout << exp.what() << std::endl;
        return -1;
    }
    config cfg; Mode mode = generatorKey; configEnc cfgEnc; configDec cfgDec;
    std::fstream in(file, std::ios_base::in);
    std::ifstream stream;
    std::ofstream result;
    std::string str;
    //stream.exceptions(std::ifstream::eofbit);
    if (!in.is_open()) {
        Logger::print<Log::Level::fatal>((std::string("Can't open file: ") + file).c_str());
        opened = false;
    } else {
        std::stringstream buf;
        buf << in.rdbuf();
        std::string json = buf.str();
        try {
            cfg = parse(json);
            Logger::setLevel(cfg.LogLevel);
            if (cfg.mode == "encrypt") mode = encrypt;
            if (cfg.mode == "decrypt") mode = decrypt;
            if (cfg.mode == "generatorKey") mode = generatorKey;

            stream.open(cfg.file, std::ios_base::binary);
            if (!stream.is_open()) {
                Logger::print<Log::Level::fatal>((std::string("Can't open file: ") + cfg.file).c_str());
                opened = false;
            }
        }
        catch (error::Exeption &exp) {
            Logger::print<Log::Level::fatal>(exp.what().c_str());
            std::cout << exp.what() << std::endl;
            opened = false;
        }
    }
    in.close();

    if(mode != generatorKey)
    {
        if(mode == encrypt)
            in.open(fileEnc, std::ios_base::in);
        else
            in.open(fileDec, std::ios_base::in);
        if (!in.is_open()) {
            Logger::print<Log::Level::fatal>((std::string("Can't open file: ") + ((mode == encrypt) ? fileEnc : fileDec)).c_str());
            opened = false;
        } else {
            std::stringstream buf;
            buf << in.rdbuf();
            std::string json = buf.str();
            try {
                if(mode == encrypt)
                    cfgEnc = parseEnc(json);
                else
                    cfgDec = parseDec(json);
            }
            catch (error::Exeption &exp) {
                Logger::print<Log::Level::fatal>(exp.what().c_str());
                std::cout << exp.what() << std::endl;
                opened = false;
            }
        }
    }

    if(!opened)
        return -1;

    if(mode == generatorKey)
    {
        std::fstream  outEnc, outDec;
        outEnc.open(fileEnc, std::ios_base::out);
        outDec.open(fileDec, std::ios_base::out);
        BigInteger::BigInt p, q, n, phi, e;

        p.setByString(cfg.p); q.setByString(cfg.q);

        {
            using namespace BigInteger;
            n = p * q;
            phi = (p - "1"_BigInt) * (q - "1"_BigInt);
            e = "11"_BigInt;
        }

        auto[gcb, d, y] = BigInteger::gcb(e, phi);
        outEnc << "{\n  \"E\" : \"" << e.toString() << "\",\n";
        outEnc << "  \"N\" : \"" << n.toString() << "\"\n}";
        outEnc.close();
        {
            using namespace BigInteger;
            while (d < "0"_BigInt)
            {
                d = d + phi;
            }
        }
        d = d % phi;
        outDec << "{\n  \"D\" : \"" << d.toString() << "\",\n";
        outDec << "  \"N\" : \"" << n.toString() << "\"\n}";
        outDec.close();
    }else if(mode == encrypt)
    {
        BigInteger::BigInt n; n.setByString(cfgEnc.n);
        Field::IntegerMod::setIntegerMod(n);
        Field::BigInt e; e.setByString(cfgEnc.e);
        std::fstream out((cfg.file + ".rsa"), std::ios::out);
        char a;
        BigInteger::BigInt buf; buf.setByString("0");
        Field::BigInt A;
        //size_t sizeBlock = n.getBitSize() - 1, local = 0;
        while(stream.get(a))
        {
            char mas[2];
            itoa(a, mas, 16);
            A.setByString(mas);
            auto res = A ^ e;
            out << res.toString() << std::endl;
        }
        out.close();
        stream.close();
    }else
    {
        BigInteger::BigInt n; n.setByString(cfgDec.n);
        Field::IntegerMod::setIntegerMod(n);
        Field::BigInt d; d.setByString(cfgDec.d);
        auto OUTPUT = cfg.file;
        OUTPUT.erase(OUTPUT.end() - 4, OUTPUT.end());
        std::fstream out( OUTPUT, std::ios::out | std::ios::binary);
        std::string str;
        while(std::getline(stream, str))
        {
            Field::BigInt M;
            str.pop_back();
            M.setByString(str);

            auto dec = M ^ d;
            char* beg = (char*)(&dec.getVector()[0]);
            out.write(beg, dec.getVector().size());
        }
        out.close();
        stream.close();
    }
}