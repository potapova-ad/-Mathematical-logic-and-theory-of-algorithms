#include <iostream>
#include "Form.hpp"
#include "Eval.hpp"

int main() {

	std::string str = "";
	std::cout << "Enter the formula:" << std::endl ;
	std::getline(std::cin, str);

	Array_of_lexem arr = Array_of_lexem(str);
    std::cout << "The result of splitting into lexem:" << std::endl;
	for (int i = 0; i < arr.size; ++i) {
        std::cout << "\"" << arr.lexems[i] << "\" " ;
	}

    std::cout << std::endl;

	Form pars(arr);

	if (pars.root == nullptr) {
        return 0;
	}
	std::cout << "Parser result: " ;
	pars.show();
	std::cout << std::endl;

	std::cout << "Eval: " ;
	Eval f(pars);
	std::cout << std::endl;

	std::cout << "Enter variable values. Example: a = 1 b = 0 c = 1 ..." << std::endl;
	std:: string s;
    std::getline(std::cin, s);
    std::cout << std::endl;
	Form eval_work = f.eval_(s);
	std::cout << "Formula value on the set of values:" << std::endl;
	eval_work.show();
	std::cout << std::endl;

    std::cout << std::endl;
	std::cout << "Does the formula belong to the language TAUT?" <<
    "If yes, then 1, otherwise a set of values of giving the value of the formula 0:"<< std::endl << "Result: " << f.taut() << std::endl;
	std::cout << "Does the formula belong to the language SAT?" <<
    "If no, then 0, otherwise a set of values of giving the value of the formula 1:" << std::endl << "Result: " << f.sat() << std::endl;

    std::cout << std::endl;
	std::cout << "cnf form:" << std::endl;
	Form *cnf = f.cnf(&pars);

	return 0;
}
