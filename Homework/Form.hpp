#pragma once
#include <iostream>


// Типы лексем:
const int f = 0, t = 1, var = 2, bracket = 3, operation = 4;

//Типы операций:
const int and_ = 5, or_ = 6, implication = 7, not_ = 8, equality = 9;

//Типы скобок:
const int open_bracket = 10, close_bracket = 11;



class Lexem_class {
public:

	int type_of_lexem = -1;
    int t_f = -1;
    std::string var = "";
    int type_of_bracket = -1;
    int type_of_operation = -1;


};


class Array_of_lexem {
public:

    Array_of_lexem(std::string str) {

        size = 0;
        capacity = 100;
        lexems = new Lexem_class [capacity];
        int i = 0;
        while (str[i] != '\0') {

            if (size >= capacity) {
                resize();
            }

            switch (str[i]) {
                case '0' :
                    lexems[size].type_of_lexem = f;
                    lexems[size].t_f = 0;
                    break;

                case '1' :
                    lexems[size].type_of_lexem = t;
                    lexems[size].t_f = 1;
                    break;

                case '(':
                    lexems[size].type_of_lexem  = bracket ;
                    lexems[size].type_of_bracket = open_bracket;
                    break;

                case ')':
                    lexems[size].type_of_lexem  = bracket;
                    lexems[size].type_of_bracket = close_bracket;
                    break;

                case '~':
                    lexems[size].type_of_lexem = operation;
                    lexems[size].type_of_operation = not_;
                    break;

                case '&' :
                    lexems[size].type_of_lexem = operation;
                    lexems[size].type_of_operation = and_;
                    break;

                case '|' :
                    lexems[size].type_of_lexem   = operation;
                    lexems[size].type_of_operation = or_;
                    break;

                case '-' :
                    if (i < capacity  && str[i + 1] != '>') {
                        std::cout << "Error! Operator \'-\' underfined!\n" ;
                        abort();
                    }

                    lexems[size].type_of_lexem = operation;
                    lexems[size].type_of_operation = implication;
                    i++;
                    break;

                case '=' :
                    lexems[size].type_of_lexem = operation;
                    lexems[size].type_of_operation = equality;
                    break;

                default:
                    if (isalpha(str[i])) {
                        std::string temp_str = "";
                        while (isdigit(str[i]) || isalpha(str[i])) {
                            temp_str += str[i];
                            ++i;
                        }
                        temp_str += "\0";
                        --i;
                        lexems[size].type_of_lexem = var;
                        lexems[size].var = temp_str;
                        ++size;
                    }

                    else if (isspace (str[i]) == 0) {
                        std::cout << "Error!";
                        abort();
                    }
                    ++i;
                    continue;
            }
            ++size;
            ++i;
        }
    }

    ~Array_of_lexem() {

	delete [] lexems;

    }

    void resize() {

        capacity *= 2;
        Lexem_class *temp = new Lexem_class [capacity];

        for (int i = 0; i < size; ++i) {

            temp[i] = lexems[i];

        }

        delete [] lexems;
        lexems = temp;
    }

	Lexem_class *lexems;
	int size;
	int capacity;

};

std::ostream &operator<< (std::ostream &left, Lexem_class &lexem) {
	switch (lexem.type_of_lexem) {
        case operation:
            switch (lexem.type_of_operation) {
                case and_:
                    std::cout << "&" ;
                    break;
                case or_:
                    std::cout << "|" ;
                    break;
                case not_:
                    std::cout << "~" ;
                    break;
                case implication:
                    std::cout << "->" ;
                    break;
                case equality:
                    std::cout << "=" ;
                    break;
                default:
                    std::cout << "<Unknown operation type>";
                    abort();
                    break;
            }
		break;

        case bracket:
            switch (lexem.type_of_bracket) {
                case open_bracket:
                    std::cout << "(" ;
                    break;
                case close_bracket:
                    std::cout << ")";
                    break;
                default:
                    std::cout << "<Unknown bracket type>";
                    abort();
                    break;
            }
		break;

        case var:
            std::cout << lexem.var ;
            break;

        case t:
            std::cout << "1" ;
            break;

        case f:
            std::cout << "0";
            break;

        default:
            std::cout << "<Unknown lexem type>" ;
            abort();
            break;
    }
	return left;
}

//Типы узлов:
const int operation_in_node = 12, var_in_node = 13, t_f_in_node = 14;


class Data_in_node {
public:

	int type_of_node = -1;
	int type_of_operation = -1;
    int t_f = -1;
    std::string var = "";
	int var_value = -1;

};

class Node {
public:

	Node() {

        left  = nullptr;
        right = nullptr;
        isbracket = 0;

    }

    Node *copy_node(Node *other) {

        if (other == nullptr)
            return nullptr;
        Node *new_node = new Node() ;
        new_node->right = copy_node(other->right);
        new_node->left  = copy_node(other->left);
        new_node->data.type_of_node = other->data.type_of_node;
        new_node->data.type_of_operation = other->data.type_of_operation;
        new_node->data.t_f = other->data.t_f;
        new_node->data.var = other->data.var;
        new_node->data.var_value = other->data.var_value;

        return new_node;
    }

	Node *left, *right;
	class Data_in_node data;
	bool isbracket;

};

Node *copy_node(Node *other);
void destroy_subtree(Node *node_);

std::ostream &operator<< (std::ostream &left, Node *other) {
    if (other == nullptr) {
        left << "Nullptr node" << std::endl;
    }

    switch (other->data.type_of_node) {
        case operation_in_node:
            switch (other->data.type_of_operation) {
                case and_:
                    left << "&";
                    break;
                case or_:
                    left << "|";
                    break;
                case implication:
                    left << "->";
                    break;
                case not_:
                    left << "~";
                    break;
                case equality:
                    left << "=";
                    break;
                default:
                    left << "Error!" << std::endl;
            }
            break;
        case t_f_in_node:
            left << other->data.t_f;
            break;
        case var_in_node:
            left << other->data.var;
            break;
        default:
            left << "Error!";
    }

    return left;
}

class Form {
public:

    Form() {
        counter = 0;
        root  = nullptr;
    }

    Form(Array_of_lexem &arr) {         //read: String -> Form
        counter = 0;
        root = nullptr;
        root = parse_expr(arr);

        if (counter != arr.size) {
            std::cout << "Error!";
            destroy_Form(root);
            root = nullptr;
            return;
        }
    }

    Form(Node *other) {
        counter = 0;
        root  = other;
    }

    ~Form() {
        destroy_Form(root);
    }

    void destroy_Form(Node *other) {

        if (other == nullptr) {
            return;
        }

        destroy_Form(other->left );
        destroy_Form(other->right);

        delete other;

        return;
    }

    void print_node(Node *other) {
        if (other == nullptr) {
            return;
        }

        if (other->data.type_of_node == operation_in_node && other->data.type_of_operation == not_) {
            std::cout << "~" ;
            print_node(other->left);
            return;
        }
        else if (other->isbracket) {
            std::cout << "(" ;
            print_node(other->left);
            std::cout << other;
            print_node(other->right);
            std::cout << ")" ;
        }
        else {
            print_node(other->left);
            std::cout << other;
            print_node(other->right);
        }
    }

    void show() {
        print_node(root);
    }

    Node *parse_expr(Array_of_lexem &arr) {

        Node *new_node;

        new_node = parse_disjunct(arr);
        if (counter >= arr.size || arr.lexems[counter].type_of_operation != implication) {
            return new_node;
        }

        Node *temp = new Node();
        temp->data.type_of_node = operation_in_node;
        temp->data.type_of_operation = implication;
        temp->left = new_node;
        temp->isbracket = 0;
        ++counter;
        temp->right = parse_expr(arr);

        return temp;
    }

    Node *parse_disjunct(Array_of_lexem &arr) {

        Node *new_node;

        new_node = parse_conjunct(arr);
        if (counter >= arr.size || arr.lexems[counter].type_of_operation != or_) {
            return new_node;
        }

        Node *temp = new Node();
        temp->data.type_of_node = operation_in_node;
        temp->data.type_of_operation = or_;
        temp->left = new_node;
        temp->isbracket = 0;
        ++counter;
        temp->right = parse_disjunct(arr);
        return temp;
    }

    Node *parse_conjunct(Array_of_lexem &arr) {

        Node *new_node = nullptr;

        new_node = parse_term(arr);
        if (counter >= arr.size || arr.lexems[counter].type_of_operation != and_) {
            return new_node;
        }

        Node *temp = new Node ();
        temp->data.type_of_node = operation_in_node;
        temp->data.type_of_operation  = and_;
        temp->left = new_node;
        temp->isbracket  = 0;
        ++counter;
        temp->right = parse_conjunct(arr);

        return temp;
    }

    Node *parse_term(Array_of_lexem &arr) {
        Node *new_node = nullptr;
        if (counter >= arr.size) {
            return nullptr;
        }
        switch (arr.lexems[counter].type_of_lexem) {
            case bracket:
                if (arr.lexems[counter].type_of_bracket == open_bracket) {
                    ++counter;
                }
                else {
                    std::cout << "Error!" ;
                    counter = arr.size + 1;
                    return nullptr;
                }

                new_node = parse_expr(arr);
                if (new_node != nullptr)
                    new_node->isbracket = 1;

                if (arr.lexems[counter].type_of_bracket == close_bracket) {
                    ++counter;
                }
                else {
                    std::cout << "Error!" ;
                    counter = arr.size + 1;
                    return nullptr;
                }
                return new_node;

            case var:
                new_node = new Node();
                new_node->data.type_of_node = var_in_node;
                new_node->data.var = arr.lexems[counter++].var;
                return new_node;
            case operation:
                if (arr.lexems[counter].type_of_operation == not_) {
                    new_node = new Node();
                    new_node->data.type_of_node = operation_in_node;
                    new_node->data.type_of_operation = arr.lexems[counter++].type_of_operation;
                    new_node->left = parse_term(arr);
                    return new_node;
                }
                printf("Error!");
                counter = arr.size + 1;
                return nullptr;

            case t:
                new_node = new Node();
                new_node->data.type_of_node = t_f_in_node;
                new_node->data.t_f = 1;
                new_node->data.var_value = 1;
                ++counter;

                return new_node;

            case f:
                new_node = new Node();
                new_node->data.type_of_node = t_f_in_node;
                new_node->data.t_f = 0;
                new_node->data.var_value = 0;
                ++counter;
                return new_node;

            default:
                std::cout << "Error!" ;
                counter = arr.size + 1;
                return nullptr;
        }
    }

    Node *copy_root (Node *root_) {
        return copy_node(root_);
    }

	Node *root ;
	int counter;
};
    Node *copy_node(Node *other) {

        if (other == nullptr)
            return nullptr;
        Node *new_node = new Node() ;
        new_node->right = copy_node(other->right);
        new_node->left  = copy_node(other->left);
        new_node->data.type_of_node = other->data.type_of_node;
        new_node->data.type_of_operation = other->data.type_of_operation;
        new_node->data.t_f = other->data.t_f;
        new_node->data.var = other->data.var;
        new_node->data.var_value = other->data.var_value;

        return new_node;
    }

    void destroy_subtree(Node *node_) {
        if (node_ == nullptr) return;

        destroy_subtree(node_->left );
        destroy_subtree(node_->right);

        delete node_;
    }
