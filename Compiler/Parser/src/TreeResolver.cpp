#include "TreeResolver.h"


namespace PARSER {
    std::vector<std::tuple<std::string, std::string, bool>>TreeResolver::CVlist = std::vector<std::tuple<std::string, std::string, bool>>(
            0);
    std::vector<std::tuple<std::string, std::string, bool>>TreeResolver::Vlist = std::vector<std::tuple<std::string, std::string, bool>>(
            0);;
    std::vector<std::vector<std::string>>TreeResolver::result = std::vector<std::vector<std::string>>(0);

    std::vector<int>TreeResolver::tEntryList = std::vector<int>(0);
    std::vector<int>TreeResolver::fEntryList = std::vector<int>(0);
    int TreeResolver::tempCount = 0;
    bool TreeResolver::isNum = 0;
    bool TreeResolver::constSituation = false;
    bool TreeResolver::inDecl = false;

    template<typename T>
    T Calculate(T a, T b, std::string sign) {
        CORE_ASSERT(sign != "/" || b != 0, "Divide by Zero Error!");
        if (sign == "+")return a + b;
        else if (sign == "-")return a - b;
        else if (sign == "*")return a * b;
        else if (sign == "/")return a / b;
        else if (sign == "&&")          if(a!=0&&b!=0)return 1;
                                        else          return 0;
        else if (sign == "||")          if(a==0&&b==0)return 0;
                                        else          return 1;
        else if (sign == ">")           return a>b;
        else if (sign == "<")           return a<b;
        else if (sign == "==")          return a==b;
        else if (sign == "!=")          return a!=b;
        else if (sign == ">=")          return a>=b;
        else if (sign == "<=")          return a<=b;
        else CORE_FATAL("UNKNOWN OPERATOR!");
        return 0;
    }

    int Calculate(int a, int b, std::string sign) {
        CORE_ASSERT(sign != "/" || b != 0, "Divide by Zero Error!");
        if (sign == "+")return a + b;
        else if (sign == "-")           return a - b;
        else if (sign == "*")           return a * b;
        else if (sign == "/")           return a / b;
        else if (sign == "%")           return a % b;
        else if (sign == "&&")          if(a!=0&&b!=0)return 1;
                                        else          return 0;
        else if (sign == "||")          if(a==0&&b==0)return 0;
                                        else          return 1;
        else if (sign == ">")           return a>b;
        else if (sign == "<")           return a<b;
        else if (sign == "==")          return a==b;
        else if (sign == "!=")          return a!=b;
        else if (sign == ">=")           return a>=b;
        else if (sign == "<=")           return a<=b;
        else CORE_FATAL("UNKNOWN OPERATOR!");
        return 0;
    }

    bool TreeResolver::resolve(ASTNode *root) {
        for (const auto &r: root->getChldren())
            resolveHelp(r, 0);
        return true;
    }

    std::tuple<bool, std::string, bool> TreeResolver::resolveHelp(ASTNode *root, int typeRes) {
        if (root->getContent() == "const") {
            constSituation = true;
            resolveHelp(root->getChldren()[0], 0);
            constSituation = false;
        } else if (root->getContent() == "int" || root->getContent() == "float") {
            inDecl = true;
            for (const auto &r: root->getChldren())
                resolveHelp(r, root->getContent() == "int" ? 1 : 2);
            inDecl = false;
        } else if (root->getIfFlag()){
            int code_now = result.size();
            auto value = resolveHelp(root->getChldren()[0],0);
            auto[isConst, Val, type] = value;
            if(isConst){
                if(Val != "0"){
                    resolveHelp(root->getChldren()[1],0);
                }
                else{
                    if(root->getChldren().size()>2){
                        resolveHelp(root->getChldren()[2],0);
                    }
                }
            }
            else{
                while (!tEntryList.empty()&&tEntryList.back()>=code_now){
                    result[tEntryList.back()][3]= std::to_string(result.size());
                    tEntryList.pop_back();
                }
                resolveHelp(root->getChldren()[1],0);
                int i = result.size();
                reportRes({"jmp","","",""});
                while (!fEntryList.empty()&&fEntryList.back()>=code_now){
                    result[fEntryList.back()][3]= std::to_string(result.size());
                    fEntryList.pop_back();
                }
                if(root->getChldren().size()>2){
                    resolveHelp(root->getChldren()[2],0);
                }
                result[i][3]= std::to_string(result.size());
            }
        }else if(root->getWhileFlag()){
            CORE_INFO(root->getChldren().size());
            int code_now = result.size();
            auto value = resolveHelp(root->getChldren()[0],0);
            auto[isConst, Val, type] = value;
            if(isConst){
                if(Val != "0"){
                    resolveHelp(root->getChldren()[1],0);
                    reportRes({"jmp","","", std::to_string(code_now)});
                }
                else{
                    return {};
                }
            }
            else{
                while (!tEntryList.empty()&&tEntryList.back()>=code_now){
                    result[tEntryList.back()][3]= std::to_string(result.size());
                    tEntryList.pop_back();
                }
                tEntryList.pop_back();
                resolveHelp(root->getChldren()[1],0);
                reportRes({"jmp","","", std::to_string(code_now)});
                while (!fEntryList.empty()&&fEntryList.back()>=code_now){
                    result[fEntryList.back()][3]= std::to_string(result.size());
                    fEntryList.pop_back();
                }
            }
        }else if(root->getBlock()) {
            for(const auto&r:root->getChldren()) {
                resolveHelp(r, 0);
            }
        }else if (root->getConstDeclFlag() || root->getDecl()) {
                auto it = std::find_if(CVlist.begin(), CVlist.end(),
                                       [&](const std::tuple<std::string, std::string, bool> p) {
                    if (std::get<0>(p) == root->getContent())
                        return true;
                    return false;
                });
                CORE_ASSERT(it == CVlist.end(), "Definition Conflict!");
                it = std::find_if(Vlist.begin(), Vlist.end(), [&](const std::tuple<std::string, std::string, bool> p) {
                    if (std::get<0>(p) == root->getContent())
                        return true;
                    return false;
                });
                CORE_ASSERT(it == Vlist.end(), "Definition Conflict!");

                auto value = resolveHelp(root->getChldren()[0], 0);
                auto[isConst, Val, type] = value;
                CORE_ASSERT(isConst || !root->getConstDeclFlag(), "Try to assign a non-const value to a const value!");

                if (root->getConstDeclFlag()) {
                    CVlist.push_back({root->getContent(), Val, typeRes == 1 ? 0 : 1});
                } else {
                    Vlist.push_back({root->getContent(), Val, typeRes == 1 ? 0 : 1});
                }
                reportRes({"=", Val, "", root->getContent()});
            }else if (root->getOpFlag()) {
            if(root->getContent()=="!"){
                auto value = resolveHelp(root->getChldren()[0],0);
                auto[isConst, Val, type] = value;
                isNum = 1;
                if(isConst)return {true,Val=="0"?"1":"0",false};
                else {
                    reportRes({"jz",Val,"",""});
                    tEntryList.push_back(result.size()-1);
                    reportRes({"jmp","","",""});
                    fEntryList.push_back(result.size()-1);
                    isNum = 0;
                    return {false,"",""};
                }
            }
            if(root->getContent()=="&&"){
                int code_now = result.size();
                auto value = resolveHelp(root->getChldren()[0],0);
                auto[isConst, Val, type] = value;
                bool isNum_ = isNum;
                if(isConst){
                    isNum = 1;
                    if(Val=="0")return {true,"0",false};
                    else {
                        auto value_ = resolveHelp(root->getChldren()[1],0);
                        auto[isConst_, Val_, type_] = value_;
                        if(isConst_){
                            if(Val_ == "0")return {true,"0",false};
                            else return {true,"1",false};
                        }
                        else if(isNum){
                            reportRes({"jz", Val_, "", ""});
                            fEntryList.push_back(result.size() - 1);
                            reportRes({"jmp", "", "", ""});
                            tEntryList.push_back(result.size() - 1);
                            isNum = 0;
                            return {false,"",""};
                        }
                        else{
                            while(!fEntryList.empty()&&fEntryList.back()>=code_now) {
                                result[tEntryList.back()][3]= std::to_string(result.size());
                                fEntryList.pop_back();
                            }
                            reportRes({"jmp", "", "", ""});
                            fEntryList.push_back(result.size() - 1);
                            while(!tEntryList.empty()&&tEntryList.back()>=code_now) {
                                result[tEntryList.back()][3]= std::to_string(result.size());
                                tEntryList.pop_back();
                            }
                            reportRes({"jmp", "", "", ""});
                            tEntryList.push_back(result.size() - 1);
                            isNum = 0;
                            return {false,"",""};
                        }
                    }
                }
                else {
                    int code_now_=result.size();
                    auto value_ = resolveHelp(root->getChldren()[1], 0);
                    auto[isConst_, Val_, type_] = value_;
                    CORE_WARN("here:"+ std::to_string(code_now)+" "+ std::to_string(code_now_));
                    if (isConst_) {
                        if (Val_ == "0") {
                            while (!tEntryList.empty() && tEntryList.back() >= code_now) {
                                result.erase(result.begin() + tEntryList.back());
                                tEntryList.pop_back();
                            }
                            while (!fEntryList.empty() && fEntryList.back() >= code_now) {
                                result.erase(result.begin() + fEntryList.back());
                                fEntryList.pop_back();
                            }
                            isNum = 1;
                            return {true, "0", false};
                        } else {
                            while (!fEntryList.empty() && fEntryList.back() >= code_now) {
                                result[fEntryList.back()][3] = std::to_string(result.size());
                                fEntryList.pop_back();
                            }
                            reportRes({"jmp", Val, "", ""});
                            fEntryList.push_back(result.size() - 1);
                            while (!tEntryList.empty() && tEntryList.back() >= code_now) {
                                result[tEntryList.back()][3] = std::to_string(result.size());
                                tEntryList.pop_back();
                            }
                            reportRes({"jmp", "", "", ""});
                            tEntryList.push_back(result.size() - 1);
                            isNum = 0;
                            return {false, "", false};
                        }
                    }
                    if(isNum&&isNum_){
                        reportRes({"jz",Val,"",""});
                        fEntryList.push_back(result.size()-1);
                        reportRes({"jz",Val_,"",""});
                        fEntryList.push_back(result.size()-1);
                        reportRes({"jmp","","",""});
                        tEntryList.push_back(result.size()-1);
                        isNum = 0;
                        return {false,"",""};
                    }
                    else if(isNum&&!isNum_){
                        while (!fEntryList.empty() && fEntryList.back() >= code_now) {
                            result[fEntryList.back()][3] = std::to_string(result.size());
                            fEntryList.pop_back();
                        }
                        reportRes({"jmp", "", "", ""});
                        fEntryList.push_back(result.size() - 1);
                        while (!tEntryList.empty() && tEntryList.back() >= code_now&& tEntryList.back()<code_now_) {
                            result[tEntryList.back()][3] = std::to_string(code_now_);
                            tEntryList.pop_back();
                        }
                        reportRes({"jz", Val_, "", ""});
                        fEntryList.push_back(result.size() - 1);
                        reportRes({"jmp","","",""});
                        tEntryList.push_back(result.size() - 1);
                        isNum = 0;
                        return {false,"",""};
                    }
                    else if(!isNum&&isNum_){
                        reportRes({"jz", Val, "", ""});
                        fEntryList.push_back(result.size() - 1);
                        while (!fEntryList.empty() && fEntryList.back() >= code_now) {
                            result[fEntryList.back()][3] = std::to_string(result.size());
                            fEntryList.pop_back();
                        }
                        reportRes({"jmp", "", "", ""});
                        fEntryList.push_back(result.size() - 1);
                        while (!tEntryList.empty() && tEntryList.back() >= code_now&& tEntryList.back()<code_now_) {
                            result[tEntryList.back()][3] = std::to_string(result.size());
                            tEntryList.pop_back();
                        }
                        reportRes({"jmp", "", "", ""});
                        tEntryList.push_back(result.size() - 1);
                        isNum = 0;
                        return {false,"",""};
                    }
                    else {
                            while (!fEntryList.empty() && fEntryList.back() >= code_now) {
                                result[fEntryList.back()][3] = std::to_string(result.size());
                                fEntryList.pop_back();
                            }
                            reportRes({"jmp", "", "", ""});
                            fEntryList.push_back(result.size() - 1);
                            while (!tEntryList.empty() && tEntryList.back() >= code_now_) {
                                result[tEntryList.back()][3] = std::to_string(result.size());
                                tEntryList.pop_back();
                            }
                            while (!tEntryList.empty() && tEntryList.back() >= code_now &&
                                   tEntryList.back() < code_now_) {
                                result[tEntryList.back()][3] = std::to_string(code_now_);
                                tEntryList.pop_back();
                            }
                            reportRes({"jmp", "", "", ""});
                            tEntryList.push_back(result.size() - 1);
                            isNum = 0;
                            return {false, "", false};
                        }
                    }
                }
            else if(root->getContent() == "||"){
                int code_now = result.size();
                auto value = resolveHelp(root->getChldren()[0],0);
                auto[isConst, Val, type] = value;
                bool isNum_ = isNum;
                if(isConst){
                    isNum = 1;
                    if(Val=="1")return {true,"1",false};
                    else {
                        auto value_ = resolveHelp(root->getChldren()[1],0);
                        auto[isConst_, Val_, type_] = value_;
                        if(isConst_){
                            if(Val_ == "1")return {true,"1",false};
                            else return {true,"0",false};
                        }
                        else if(isNum){
                            reportRes({"jnz", Val_, "", ""});
                            tEntryList.push_back(result.size() - 1);
                            reportRes({"jmp", "", "", ""});
                            fEntryList.push_back(result.size() - 1);
                            isNum = 0;
                            return {false,"",""};
                        }
                        else{
                            while(!fEntryList.empty()&&fEntryList.back()>=code_now) {
                                result[tEntryList.back()][3]= std::to_string(result.size());
                                fEntryList.pop_back();
                            }
                            reportRes({"jmp", "", "", ""});
                            fEntryList.push_back(result.size() - 1);
                            while(!tEntryList.empty()&&tEntryList.back()>=code_now) {
                                result[tEntryList.back()][3]= std::to_string(result.size());
                                tEntryList.pop_back();
                            }
                            reportRes({"jmp", "", "", ""});
                            tEntryList.push_back(result.size() - 1);
                            isNum = 0;
                            return {false,"",""};
                        }
                    }
                }
                else {
                    int code_now_=result.size();
                    auto value_ = resolveHelp(root->getChldren()[1], 0);
                    auto[isConst_, Val_, type_] = value_;
                    if (isConst_) {
                        if (Val_ == "1") {
                            while (!tEntryList.empty() && tEntryList.back() >= code_now) {
                                result.erase(result.begin() + tEntryList.back());
                                tEntryList.pop_back();
                            }
                            while (!fEntryList.empty() && fEntryList.back() >= code_now) {
                                result.erase(result.begin() + fEntryList.back());
                                fEntryList.pop_back();
                            }
                            isNum = 1;
                            return {true, "1", false};
                        } else {
                            while (!tEntryList.empty() && tEntryList.back() >= code_now) {
                                result[tEntryList.back()][3] = std::to_string(result.size());
                                tEntryList.pop_back();
                            }
                            reportRes({"jmp", Val, "", ""});
                            tEntryList.push_back(result.size() - 1);
                            while (!fEntryList.empty() && fEntryList.back() >= code_now) {
                                result[fEntryList.back()][3] = std::to_string(result.size());
                                fEntryList.pop_back();
                            }
                            reportRes({"jmp", "", "", ""});
                            fEntryList.push_back(result.size() - 1);
                            isNum = 0;
                            return {false, "", false};
                        }
                    }
                    if(isNum&&isNum_){
                        reportRes({"jnz",Val,"",""});
                        tEntryList.push_back(result.size()-1);
                        reportRes({"jnz",Val_,"",""});
                        tEntryList.push_back(result.size()-1);
                        reportRes({"jmp","","",""});
                        fEntryList.push_back(result.size()-1);
                        isNum = 0;
                        return {false,"",""};
                    }
                    else if(isNum&&!isNum_){
                        while (!tEntryList.empty() && tEntryList.back() >= code_now) {
                            result[fEntryList.back()][3] = std::to_string(result.size());
                            tEntryList.pop_back();
                        }
                        reportRes({"jmp", "", "", ""});
                        tEntryList.push_back(result.size() - 1);
                        while (!fEntryList.empty() && fEntryList.back() >= code_now&& fEntryList.back()<code_now_) {
                            result[tEntryList.back()][3] = std::to_string(code_now_);
                            fEntryList.pop_back();
                        }
                        reportRes({"jnz", Val_, "", ""});
                        tEntryList.push_back(result.size() - 1);
                        reportRes({"jmp","","",""});
                        fEntryList.push_back(result.size() - 1);
                        isNum = 0;
                        return {false,"",""};
                    }
                    else if(!isNum&&isNum_){
                        reportRes({"jnz", Val, "", ""});
                        tEntryList.push_back(result.size() - 1);
                        while (!fEntryList.empty() && fEntryList.back() >= code_now) {
                            result[fEntryList.back()][3] = std::to_string(result.size());
                            fEntryList.pop_back();
                        }
                        reportRes({"jmp", "", "", ""});
                        fEntryList.push_back(result.size() - 1);
                        while (!tEntryList.empty() && tEntryList.back() >= code_now&& tEntryList.back()<code_now_) {
                            result[tEntryList.back()][3] = std::to_string(result.size());
                            tEntryList.pop_back();
                        }
                        reportRes({"jmp", "", "", ""});
                        tEntryList.push_back(result.size() - 1);
                        isNum = 0;
                        return {false,"",""};
                    }
                    else  {
                            while (!tEntryList.empty() && tEntryList.back() >= code_now) {
                                result[tEntryList.back()][3] = std::to_string(result.size());
                                tEntryList.pop_back();
                            }
                            reportRes({"jmp", "", "", ""});
                            tEntryList.push_back(result.size() - 1);
                            while (!fEntryList.empty() && fEntryList.back() >= code_now &&
                                   fEntryList.back() < code_now_) {
                                result[fEntryList.back()][3] = std::to_string(code_now_);
                                fEntryList.pop_back();
                            }
                            while (!fEntryList.empty() && fEntryList.back() >= code_now_) {
                                result[fEntryList.back()][3] = std::to_string(result.size() - 1);
                                fEntryList.pop_back();
                            }
                            reportRes({"jmp", "", "", ""});
                            tEntryList.push_back(result.size() - 1);
                            isNum = 0;
                            return {false, "", false};
                        }
                    }
                }

            if (root->getContent() == "%")
                typeRes = 1;
            auto value = resolveHelp(root->getChldren()[0], typeRes);
            auto[isConst, Val, type] = value;
            auto value_ = resolveHelp(root->getChldren()[1], typeRes);
            auto[isConst_, Val_, type_] = value_;
            if (typeRes && (type_ || type)) CORE_FATAL("this should be an int!");
            std::string op = root->getContent();
            if(op=="=="||op=="!="||op=="<"||op==">"||op==">="||op=="<="){
                if(isConst && isConst_){
                    if (type && type_) {
                        return {true, std::to_string(Calculate(atof(Val.c_str()), atof(Val_.c_str()), root->getContent())),
                                false};
                    } else if (type && !type_) {
                        return {true, std::to_string(
                                Calculate(atof(Val.c_str()), (double) atoi(Val_.c_str()), root->getContent())), false};
                    } else if (!type && type_) {
                        return {true, std::to_string(
                                Calculate((double) atoi(Val.c_str()), atof(Val_.c_str()), root->getContent())), false};
                    } else {
                        return {true, std::to_string(Calculate(atoi(Val.c_str()), atoi(Val_.c_str()), root->getContent())),
                                false};
                    }
                }
                else{
                    if(op == "=="||op=="!="){
                        reportRes({"-",Val,Val_, std::to_string(tempCount++)+"_temp"});
                        Vlist.push_back({std::to_string(tempCount-1) + "_temp", "", !type_ && !type ? 0 : 1});

                        if(op == "=="){
                            reportRes({"jz",std::to_string(tempCount-1)+"_temp","",""});
                            tEntryList.push_back(result.size()-1);
                        }
                        else{
                            reportRes({"jnz",std::to_string(tempCount-1)+"_temp","",""});
                            tEntryList.push_back(result.size()-1);
                        }
                        reportRes({"jmp","","",""});
                        fEntryList.push_back(result.size()-1);
                    }
                    else if(op==">"){
                        reportRes({"jg",Val,Val_,""});
                        tEntryList.push_back(result.size()-1);
                        reportRes({"jmp","","",""});
                        fEntryList.push_back(result.size()-1);
                    }
                    else if(op=="<"){
                        reportRes({"jl",Val,Val_,""});
                        tEntryList.push_back(result.size()-1);
                        reportRes({"jmp","","",""});
                        fEntryList.push_back(result.size()-1);
                    }else if(op=="<="){
                        reportRes({"jle",Val,Val_,""});
                        tEntryList.push_back(result.size()-1);
                        reportRes({"jmp","","",""});
                        fEntryList.push_back(result.size()-1);
                    }else if(op==">="){
                        reportRes({"jge",Val,Val_,""});
                        tEntryList.push_back(result.size()-1);
                        reportRes({"jmp","","",""});
                        fEntryList.push_back(result.size()-1);
                    }
                    isNum = 0;
                    return {false,"",""};
                }
            }

            if (constSituation) {
                CORE_ASSERT(isConst, "{0} should be a const value!", Val);
                CORE_ASSERT(isConst_, "{0} should be a const value!", Val_);
            }
            isNum = 1;
            if (isConst && isConst_) {
                //CORE_INFO("{0},{1}",Val,Val_);
                if (type && type_) {
                    return {true, std::to_string(Calculate(atof(Val.c_str()), atof(Val_.c_str()), root->getContent())),
                            true};
                } else if (type && !type_) {
                    return {true, std::to_string(
                            Calculate(atof(Val.c_str()), (double) atoi(Val_.c_str()), root->getContent())), true};
                } else if (!type && type_) {
                    return {true, std::to_string(
                            Calculate((double) atoi(Val.c_str()), atof(Val_.c_str()), root->getContent())), true};
                } else {
                    return {true, std::to_string(Calculate(atoi(Val.c_str()), atoi(Val_.c_str()), root->getContent())),
                            false};
                }
            } else {
                Vlist.push_back({std::to_string(tempCount++) + "_temp", "", !type_ && !type ? 0 : 1});
                reportRes({root->getContent(), Val, Val_, std::to_string(tempCount) + "_temp"});
                return {false, std::to_string(tempCount) + "_temp", !type_ && !type ? 0 : 1};
            }
        } else if (root->getInt()) {
            isNum = 1;
            return {true, root->getContent(), 0};
        } else if (root->getFloat()) {
            isNum = 1;
            if (typeRes == 1) CORE_FATAL("this should be an int!");
            return {true, root->getContent(), 1};
        } else {
            isNum = 1;
            auto it = std::find_if(CVlist.begin(), CVlist.end(),
                                   [&](const std::tuple<std::string, std::string, bool> p) {
                                       if (std::get<0>(p) == root->getContent())
                                           return true;
                                       return false;
                                   });
            if (!(it == CVlist.end())) {
                return {true, std::get<1>((*it)), std::get<2>((*it))};
            }
            it = std::find_if(Vlist.begin(), Vlist.end(), [&](const std::tuple<std::string, std::string, bool> p) {
                if (std::get<0>(p) == root->getContent())
                    return true;
                return false;
            });
            if (!(it == Vlist.end())) {
                return {false, root->getContent(), std::get<2>((*it))};
            }

            CORE_FATAL("undefined indent!");
            return {};
        }
        return {};
    }

    void TreeResolver::reportRes(std::vector<std::string> res) {
        result.push_back(res);
        CORE_INFO("({0},{1},{2},{3})", res[0], res[1] == "" ? " " : res[1], res[2] == "" ? " " : res[2],
                  res[3] == "" ? " " : res[3]);
    }
}
