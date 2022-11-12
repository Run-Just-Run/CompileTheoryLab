#include "TreeResolver.h"


namespace PARSER {
    std::vector<std::tuple<std::string, std::string, bool>>TreeResolver::CVlist = std::vector<std::tuple<std::string, std::string, bool>>(
            0);
    std::vector<std::tuple<std::string, std::string, bool>>TreeResolver::Vlist = std::vector<std::tuple<std::string, std::string, bool>>(
            0);;
    std::vector<std::vector<std::string>>TreeResolver::result = std::vector<std::vector<std::string>>(0);
    int TreeResolver::tempCount = 0;
    bool TreeResolver::constSituation = false;
    bool TreeResolver::inDecl = false;

    template<typename T>
    T Calculate(T a, T b, std::string sign) {
        if (sign == "+")return a + b;
        else if (sign == "-")return a - b;
        else if (sign == "*")return a * b;
        else if (sign == "/")return a / b;
        else CORE_FATAL("UNKNOWN OPERATOR!");
        return 0;
    }

    int Calculate(int a, int b, std::string sign) {
        if (sign == "+")return a + b;
        else if (sign == "-")return a - b;
        else if (sign == "*")return a * b;
        else if (sign == "/")return a / b;
        else if (sign == "%")return a % b;
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
        } else if (root->getConstDeclFlag() || root->getDecl()) {
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
        } else if (root->getOpFlag()) {
            if (root->getContent() == "%")
                typeRes = 1;

            auto value = resolveHelp(root->getChldren()[0], typeRes);
            auto[isConst, Val, type] = value;
            auto value_ = resolveHelp(root->getChldren()[1], typeRes);
            auto[isConst_, Val_, type_] = value_;
            if (typeRes && (type_ || type)) CORE_FATAL("this should be an int!");

            if (constSituation) {
                CORE_ASSERT(isConst, "{0} should be a const value!", Val);
                CORE_ASSERT(isConst_, "{0} should be a const value!", Val_);
            }

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
            return {true, root->getContent(), 0};
        } else if (root->getFloat()) {
            if (typeRes == 1) CORE_FATAL("this should be an int!");
            return {true, root->getContent(), 1};
        } else {
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
                return {false, std::get<1>((*it)), std::get<2>((*it))};
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
