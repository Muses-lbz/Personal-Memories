#pragma one

class ReferenceCounter{
public:
	ReferenceCounter();
	~ReferenceCounter();

	void increase();
	void decrease();
	int getCount() const;

private:
	int * count;
};

template <typename T>
class SmartPointer{
public:
	SmartPointer();
	SmartPointer(T* ptr);
	SmartPointer(const SmartPointer& other);
	SmartPointer(SmartPointer&& other) noexcept;
	~SmartPointer();

	T& operator*() const;
	T* operator->() const;

	SmartPointer&operator = (const SmartPointer& other);
	SmartPointer&operator = (SmartPointer&&other) noexcept;

private:
	T* data;
	ReferenceCounter* refCounter;
};

template <typename T>
SmartPointer<T>::SmartPointer():data(nullptr),refCounter(nullptr){}

template <typename T>
SmartPointer<T>::SmartPointer(T* ptr):data(ptr),refCounter(new ReferenceCounter()){
	refCounter->increase();
}

template <typename T>
SmartPointer<T>::SmartPointer(const SmartPointer&other):data(other.data),refCounter(other.refCounter){
	refCounter->increase();
}

template <typename T>
SmartPointer<T>::SmartPointer(SmartPointer && other) noexcept:data(other.data),refCounter(other.refCounter){
	other.data = nullptr;
	other.refCounter = nullptr;
}

template <typename T>
SmartPointer<T>::~SmartPointer(){
	if(refCounter){
		refCounter->decrease();
		if(refCounter->getCount() == 0){
			delete data;
			delete refCounter;
		}
	}
}

template <typename T>
T & SmartPointer<T>::operator*()const{
	return *data;
}

template <typename T>
T * SmartPointer<T>::operator->()const{
	return data;
}

template <typename T>
SmartPointer<T> & SmartPointer<T>::operator=(const SmartPointer&other){
	if(this != &other){
		if(refCounter){
			refCounter->decrease();
			if(refCounter->getCount() == 0){
				delete data;
				delete refCounter;
			}
		}

		data = other.data;
		refCounter = other.refCounter;
		refCounter->increase();
	}
	return *this;
}

template <typename T>
SmartPointer<T> & SmartPointer<T>::operator=(SmartPointer&&other) noexcept {
	if(this != &other){
		if(refCounter){
			refCounter->decrease();
			if(refCounter->getCount() == 0){
				delete data;
				delete refCounter;
			}
		}

		data = other.data;
		refCounter = other.refCounter;

		other.data = nullptr;
		other.refCounter = nullptr;
	}
	return *this;
}
