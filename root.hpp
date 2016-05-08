
#include "prototypes.hpp"
#include <string>

class root : public object_proto
{
    private:
		std::string CUR_DIR;//current directory
		std::string PREV_DIR;//previous directory
        uint32_t func_number=0;
        
        bool check_hierarchy(real_types prev, real_types next);
        int check_func_arg_types(func* fp, real_func* f, real_types* ret);
        int check_obj_childrens(std::vector<object>* obj, std::vector<real_func*> * pointers_tree, real_types* rt);
        
        int process_object(std::vector<object>* obj);//return error code
        virtual void init_methods() final;
        virtual void init_childrens() final;
        int delete_false_functions(uint32_t arg_num, real_types type_val, 
                                 std::vector<std::vector<arg_proto>*>* arg_prototypes, 
                                 std::vector<arg_dependencies>* ad, uint32_t func_num);
        gen_function exit, help, pwd, ls, cd;
        /*int exit(std::vector<arg> *in, value_type* retval);
        int help(std::vector<arg> *in, value_type* retval);
        int pwd (std::vector<arg> *in, value_type* retval);
        int ls  (std::vector<arg> *in, value_type* retval);
        int cd  (std::vector<arg> *in, value_type* retval);*/
    public:
        root() : object_proto("root", nullptr, real_types::VOID)
        {
			init_methods();
			init_childrens();
		}
        ~root();
        void exec_conveyor(std::vector<conveyor>* c);
};
