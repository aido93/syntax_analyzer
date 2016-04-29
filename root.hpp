
#include "prototypes.hpp"
#include <string>

class root : public object_proto
{
    private:
		std::string CUR_DIR;//current directory
		std::string PREV_DIR;//previous directory
        void exec_function(func* f);
        void ret_object(std::vector<object>* obj);
        virtual void init_methods() final;
        virtual void init_childrens() final;
        int exit(std::vector<arg> *in);
        int help(std::vector<arg> *in);
        int pwd(std::vector<arg> *in);
        int ls(std::vector<arg> *in);
    public:
        root() : object_proto("root", nullptr)
        {
			init_methods();
			init_childrens();
		}
        ~root();
        void exec_conveyor(std::vector<conveyor>* c);
};
