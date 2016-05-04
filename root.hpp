
#include "prototypes.hpp"
#include <string>

class root : public object_proto
{
    private:
		std::string CUR_DIR;//current directory
		std::string PREV_DIR;//previous directory
        
        int check_func_arg_types(func* fp, real_types* ret);
        int check_obj_childrens(std::vector<object>* obj, std::vector<real_func> * pointers_tree, real_types* rt);
        
        int process_object(std::vector<object>* obj);//return error code
        virtual void init_methods() final;
        virtual void init_childrens() final;
        gen_function exit, help, pwd, ls, cd;
        /*int exit(std::vector<arg> *in, rettype* retval);
        int help(std::vector<arg> *in, rettype* retval);
        int pwd (std::vector<arg> *in, rettype* retval);
        int ls  (std::vector<arg> *in, rettype* retval);
        int cd  (std::vector<arg> *in, rettype* retval);*/
    public:
        root() : object_proto("root", nullptr, real_types::VOID)
        {
			init_methods();
			init_childrens();
		}
        ~root();
        void exec_conveyor(std::vector<conveyor>* c);
};
