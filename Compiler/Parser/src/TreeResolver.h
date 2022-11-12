#pragma once

#include <vector>
#include <tuple>

#include "ASTNode.h"
#include "../../log.h"

namespace PARSER {

    class TreeResolver {
    public:
        static bool resolve(ASTNode *root);
        static const std::vector<std::vector<std::string>>&getResult(){
            return result;
        }
    private:
        static std::vector<std::tuple<std::string, std::string, bool>>CVlist;
        static std::vector<std::tuple<std::string, std::string, bool>>Vlist;

        static std::vector<std::vector<std::string>>result;
        static int tempCount;
        static bool constSituation;
        static bool inDecl;

    private:
        static std::tuple<bool, std::string, bool> resolveHelp(ASTNode *root, int typeRes);
        static void reportRes(std::vector<std::string>);
    };
}


