#include "../include/SmartPointer.h"

ReferenceCounter::ReferenceCounter():count(new int(1)){}

ReferenceCounter::~ReferenceCounter(){
	delete count;
}

void ReferenceCounter::increase(){
	(*count)++;
}

void ReferenceCounter::decrease(){
	(*count)--;
}

int ReferenceCounter::getCount() const{
	return *count;
}
