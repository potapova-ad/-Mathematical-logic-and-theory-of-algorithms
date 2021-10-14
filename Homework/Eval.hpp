#pragma once
#include <iostream>
#include <list>
#include <unordered_map>
#include <algorithm>
#include "Form.hpp"

int fl = 0;

void construct_list(Node *node_, std::list < std::string > * l);

class Eval {
public:

    Eval(Form &form_) {

        formula = new Form();
        formula->counter = form_.counter;
        formula->root = form_.copy_root(form_.root);
        formula->root = simplification_of_eval(formula->root) ;
        formula->root = application_of_de_Morgan_laws(formula->root);
        formula->show();

    }

    ~Eval() {

        delete formula;

    }

    Node *simplification_of_eval(Node *node_) {

        if (node_ == nullptr) {

                return nullptr;

        }

        if (node_->data.type_of_node != operation_in_node) {

                return node_;
        }

        if (node_->data.type_of_operation == not_) {

            if (node_->left->data.type_of_node == operation_in_node && node_->left->data.type_of_operation == not_) {

                Node *temp = node_->left->left;
                delete node_->left;
                delete node_;
                node_ = temp;
                return simplification_of_eval(node_);
            }

            Node *temp = simplification_of_eval(node_->left);

            if (temp->data.type_of_node != t_f_in_node) {

                node_->left = temp;
                return node_;

            }

            Node *new_node = new Node ();
            new_node->data.type_of_node = t_f_in_node;
            new_node->data.t_f = 1 - temp->data.t_f;
            new_node->data.var_value = new_node->data.t_f ;
            new_node->isbracket = true;

            delete temp;
            delete node_;

            return new_node;
        }

        if (node_->data.type_of_operation == implication) {

            node_->data.type_of_operation  = or_;
            Node *new_node = new Node ();
            new_node->data.type_of_node = operation_in_node;
            new_node->left = node_->left;
            new_node->data.type_of_operation = not_;
            new_node->isbracket = true;
            node_->left = new_node;

            return simplification_of_eval(node_);
        }

        Node *x1 = simplification_of_eval(node_->left), *x2 =  simplification_of_eval(node_->right) ;

        if (x1->data.type_of_node != t_f_in_node || x2->data.type_of_node != t_f_in_node) {

            if (node_->data.type_of_operation == and_ && (x1->data.var_value == 0 || x2->data.var_value == 0)) {

                Node *new_node = new Node ();
                new_node->data.type_of_node = t_f_in_node;
                new_node->data.t_f = 0;
                new_node->data.var_value = 0;
                new_node->isbracket  = true;

                delete node_;
                destroy_subtree(x1);
                destroy_subtree(x2);
                return new_node;
            }

            if (node_->data.type_of_operation == and_ && (x1->data.var_value == 1 || x2->data.var_value == 1)) {

                if (x1->data.var_value == 1)  {

                    delete x1;
                    delete node_;
                    return x2;

                }

                else {
                    delete x2;
                    delete node_;
                    return x1;
                }
            }

            if ((node_->data.type_of_operation == or_   && (x1->data.var_value == 1 || x2->data.var_value == 1)) || (node_->data.type_of_operation == implication && (x1->data.var_value == 0 || x2->data.var_value == 1))) {
                    Node *new_node = new Node ();
                    new_node->data.type_of_node = t_f_in_node;
                    new_node->data.t_f = 1;
                    new_node->data.var_value = 1;
                    new_node->isbracket = true;
                    delete node_;
                    destroy_subtree(x1);
                    destroy_subtree(x2);
                    return new_node;
            }

            if (node_->data.type_of_operation == or_ && (x1->data.var_value == 0 || x2->data.var_value == 0)) {

                if (x1->data.var_value == 0) {

                    delete x1;
                    delete node_;
                    return x2;

                }

                else {
                    delete x2;
                    delete node_;
                    return x1;
                }
            }

            if (node_->data.type_of_operation == implication && x1->data.var_value == 1) {

                delete x1;
                delete node_;
                return x2;
            }

            if (node_->data.type_of_operation == implication && x2->data.var_value == 0) {

                Node *new_node = new Node();
                new_node->left = x1;
                new_node->data.type_of_node = operation_in_node;
                new_node->data.type_of_operation = not_;
                x1->isbracket = true;
                delete x2;
                delete node_;

                return new_node;
            }

            node_->left  = x1;
            node_->right = x2;
            node_->isbracket = true;
            return node_;
        }

        Node *new_node = new Node();
        new_node->data.type_of_node = t_f_in_node;

        switch (node_->data.type_of_operation) {
            case and_:
                new_node->data.t_f = ((x1->data.t_f) & (x2->data.t_f));
                break;

            case or_:

                new_node->data.t_f = ((x1->data.t_f) | (x2->data.t_f));
                break;

            case implication:

                if (x1->data.t_f && !x2->data.t_f) {

                    new_node->data.t_f = 0;

                }
                else {

                    new_node->data.t_f = 1;

                }

                break;
        }

        new_node->data.var_value = new_node->data.t_f ;
        new_node->isbracket  = true;

        delete node_;
        delete x1;
        delete x2;
        node_ = new_node;
        return new_node;

    }

    Node *filling_data_in_node_from_table(Node *node_, std::unordered_map < std::string, bool > table) {
        if (node_ == nullptr) {

                return nullptr;
        }

        if (node_->data.type_of_node == var_in_node && table.find(std::string(node_->data.var)) != table.end()) {

                node_->data.type_of_node = t_f_in_node;
                node_->data.t_f = table[std::string(node_->data.var)] ;
                node_->data.var_value = node_->data.t_f ;
        }

        node_->left = filling_data_in_node_from_table(node_->left , table);
        node_->right = filling_data_in_node_from_table(node_->right, table);

        return node_;
    }

    Form eval_(std::string str) {

        Array_of_lexem arr (str);
        std::unordered_map < std::string, bool > table;

        int counter = 0;
        while (counter < arr.size - 2) {

            switch (arr.lexems[counter].type_of_lexem) {
                case var:
                    ++counter;
                    if (arr.lexems[counter].type_of_lexem == operation && arr.lexems[counter].type_of_operation == equality)
                        ++counter;
                    else {
                        std::cout << "Error!"<< std::endl;
                        exit(-1);
                    }
                    if (arr.lexems[counter].type_of_lexem == t || arr.lexems[counter].type_of_lexem == f)
                        table[std::string(arr.lexems[counter - 2].var)] = arr.lexems[counter].t_f;
                    else {
                        std::cout << "Error!"<< std::endl;
                        exit(-1);
                    }
                    ++counter;
                    break;

                default:

                    std::cout << "Error!"<< std::endl;
                    exit(-1);
            }
        }

        Node *eval_root = formula->copy_root(formula->root);
        eval_root = filling_data_in_node_from_table (eval_root, table);
        eval_root = simplification_of_eval(eval_root);
        Form res(eval_root);

        return res;
    }


    std::list < std::string > *construct_var_list(Node *root) {
        std::list < std::string > *new_list = new std::list < std::string > ();
        construct_list(root, new_list);
        return new_list;
    }

    bool taut() {
        std::list < std::string > * var_list = construct_var_list(formula->root);
        int var_count = var_list->size();
        bool *f = new bool [var_count + 1] {};
        std::string temp = "" ;

        while (f[var_count] != 1) {
            int i = 0;
            temp = "";

            for (auto it: *var_list) {
                temp += it + '=' + (char)(f[i] + '0') + ' ';
                ++i;
            }

            Form res = eval_(temp);
            if (!res.root->data.var_value) {
                std::cout << temp << "=> ";
                delete [] f;
                return false;
            }

            for (int i = 0; i <= var_count; ++i) {
                if (f[i] == 0) {
                    f[i] = 1;
                    break;
                }
                else f[i] = 0;
            }
        }

        delete [] f;
        return true;
    }


    bool sat() {
        std::list < std::string > * var_list = construct_var_list(formula->root);
        int var_count = var_list->size();
        bool *f = new bool [var_count + 1] {};
        std::string temp = "" ;

        while (f[var_count] != 1) {
            int i = 0;
            temp = "";

            for (auto it: *var_list) {
                temp += it + '=' + (char)(f[i] + '0') + ' ';
                ++i;
            }

            Form res = eval_(temp);
            if (res.root->data.var_value) {
                std::cout << temp << "=> " ;
                delete [] f;
                return true;
            }

            for (int i = 0; i <= var_count; ++i) {
                if (f[i] == 0) {
                    f[i] = 1;
                    break;
                }
                else f[i] = 0;
            }
        }

        delete [] f;
        return false;
    }

    Node *application_of_de_Morgan_laws(Node *node_) {

        if (node_ == nullptr) {

                return nullptr;
        }

        node_->left  = application_of_de_Morgan_laws(node_->left);

        if (node_->data.type_of_node == operation_in_node && node_->data.type_of_operation == not_)  {

            Node *temp = node_->left;
            delete node_;
            return de_Morgan_laws(temp);

        }

        node_->right = application_of_de_Morgan_laws(node_->right);

        return node_;
    }

    Node *de_Morgan_laws (Node *node_) {
        if (node_ == nullptr) {

                return nullptr;
        }

        if (node_->data.type_of_node == operation_in_node) {

            if (node_->data.type_of_operation == and_) {

                node_->data.type_of_operation = or_;

            }

            else if (node_->data.type_of_operation == or_) {

                node_->data.type_of_operation = and_;

            }

            else if (node_->data.type_of_operation == not_) {

                Node *temp = node_->left;
                delete node_;
                return temp;

            }

            node_->left  = de_Morgan_laws(node_->left);
            node_->right = de_Morgan_laws(node_->right);
            return node_;
        }

        Node *new_node = new Node();
        new_node->data.type_of_node = operation_in_node;
        new_node->data.type_of_operation = not_;
        new_node->left = node_;
        new_node->isbracket = true;
        node_ = new_node;

        return node_;
    }

    Node *application_of_law_of_distribution(Node *node_) {

        if (node_ == nullptr) {

                return nullptr;
        }

        bool need = 0;

        Node *left_ch, *right_ch, *t_node;

        if (node_->data.type_of_node == operation_in_node && node_->data.type_of_operation == or_) {

            if (node_->left->data.type_of_node == operation_in_node && node_->left->data.type_of_operation == and_) {

                left_ch = node_->left->left;
                right_ch = node_->left->right;
                t_node = node_->right;
                need = 1;
            }

            else if (node_->right->data.type_of_node == operation_in_node && node_->right->data.type_of_operation == and_) {

                left_ch = node_->right->left ;
                right_ch = node_->right->right;
                t_node = node_->left;
                need = 1;
            }
        }

        if (need) {

            fl = 1;

            Node *n_node_left = new Node ();

            n_node_left->isbracket = true;
            n_node_left->data.type_of_node = operation_in_node;
            n_node_left->data.type_of_operation = or_;
            n_node_left->left = left_ch;
            n_node_left->right = t_node ;

            Node *n_node_right = new Node ();

            n_node_right->isbracket = true;

            n_node_right->data.type_of_node = operation_in_node;
            n_node_right->data.type_of_operation = or_;
            n_node_right->left = t_node ;
            n_node_right->right = right_ch;

            node_->data.type_of_operation  = and_;
            node_->left = n_node_left;
            node_->right = n_node_right;
        }

        node_->left  = application_of_law_of_distribution(node_->left);
        node_->right = application_of_law_of_distribution(node_->right);

        return node_;
    }

    Form *cnf(Form *form_) {

        Form * cnf = new Form();

        cnf->counter = form_->counter ;
        cnf->root = form_->copy_root(form_->root);
        cnf->root = simplification_of_eval (cnf->root) ;
        cnf->root = application_of_de_Morgan_laws(cnf->root);

        while (cnf->root->data.type_of_node == operation_in_node && cnf->root->data.type_of_operation == or_) {

            fl = 0;
            cnf->root = application_of_law_of_distribution(cnf->root);

            if (!fl) {

                break;

            }

        }

        cnf->show();

        return cnf;
    }

	Form *formula;

};

void construct_list(Node *node_, std::list < std::string > * l) {

	if (node_ == nullptr) {

            return;
	}

	if (node_->data.type_of_node == var_in_node && std::find(l->begin(), l->end(), std::string(node_->data.var)) == l->end()) {

			l->push_back(node_->data.var);

	}
	construct_list(node_->left, l);
	construct_list(node_->right, l);
}

























