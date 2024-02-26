#include "../include/SmartPointer.h"
#include <iostream>

class MyClass{
public:
	MyClass(int val):value(val){}
	void print() const {
		if(this != nullptr){
			std::cout << "value:" << value << std::endl;
		}else{
			std::cout << "Pointer is nullptr" << std::endl;
		}
	}

private:
	int value;
};

int main(){
	SmartPointer<MyClass> ptr1(new MyClass(42));
	SmartPointer<MyClass> ptr2 = ptr1;
	SmartPointer<MyClass> ptr3 = std::move(ptr1);

	ptr1->print();
	ptr2->print();
	ptr3->print();

	SmartPointer<MyClass> ptr4(new MyClass(99));
	ptr4 = ptr2;

	ptr2->print();
	ptr4->print();

	SmartPointer<MyClass> ptr5 = std::move(ptr4);

	ptr4->print();
	ptr5->print();

	return 0;
}
