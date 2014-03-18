#include <iostream>
#include <cmath>
#include <vector>
#include <typeinfo> // for typeid
#include <string>
#include <boost/thread.hpp>
namespace std
{
   using boost::mutex;
   using boost::recursive_mutex;
   using boost::lock_guard;
   using boost::condition_variable;
   using boost::unique_lock;
   using boost::thread;
}

//#include <cstddef>

#include <stdio.h>

using namespace std;
void keyword_auto();
void keyword_bitand();
void keyword_bitor();
void keyword_catch();
void keyword_type();
void keyword_class();
void keyword_compl();
void keyword_const();
void keyword_constexpr(int a);
void keyword_const_cast();
void keyword_cast1();
void keyword_cast2();
void keyword_refto();
void keyword_default();
void keyword_abstract();
void keyword_cast();
void keyword_enum();
void keyword_explicit();
void keyword_nullptr();
void keyword_protected();
void keyword_static();
void keyword_template();
void keyword_thread_local();


int main() {
//	keyword_auto();
//	keyword_bitand();
//	keyword_bitor();
//	keyword_catch();
//	keyword_type();
//	keyword_class();
//	keyword_compl();
//	keyword_const();
//	keyword_constexpr(12);
//	keyword_const_cast();
//	keyword_cast1();
//	keyword_cast2();
//	keyword_refto();
//	keyword_default();
//	keyword_abstract();
//	keyword_cast();
//	keyword_enum();
//	keyword_explicit();
//	keyword_nullptr();
//	keyword_protected();
//	keyword_static();
//	keyword_template();
	keyword_thread_local();

	return 0;
}

class Animal {
	private:
		string name;
	public:
		Animal();
		~Animal();
		void setName(string name);
		string getName();
};

class Person {
	private:
		string lastname;
		string firstname;
		int age;
		double heigth;

	public:
		Animal pet;
		Person(string firstname);
		~Person();
		void setLastname(string l);
		string getLastname();
		string getFirstname();
		int getAge();
};

template<class T, class U>
auto add(T t, U u) -> decltype(t + u) // the return type of add is the type of operator+(T,U)
{
    return t + u;
}

auto get_fun(int arg)->double(*)(double) // same as double (*get_fun(int))(double)
{
    switch (arg) {
        case 1: return std::fabs;
        case 2: return std::sin;
        default: return std::cos;
    }
}

void keyword_auto() {
	cout << "\nKeyword auto\n";
	// auto
	int i = 0;
	std::cout << "type of i: " << typeid(i).name() << '\n';
	auto a = 1 + 2;
	std::cout << "type of a: " << typeid(a).name() << '\n';
	auto b = add(1, 1.2);
	std::cout << "type of b: " << typeid(b).name() << '\n';
	//auto int c; //compile-time error
	auto d = {1, 2};
	std::cout << "type of d: " << typeid(d).name() << '\n';

	auto my_lambda = [](int x) { return x + 3; };
	std::cout << "my_lambda: " << my_lambda(5) << '\n';

	auto my_fun = get_fun(2);
	std::cout << "type of my_fun: " << typeid(my_fun).name() << '\n';
	std::cout << "my_fun: " << my_fun(3) << '\n';
}

void keyword_catch() {
	cout << "\nKeyword catch\n";
	try {
		std::cout << "Throwing an integer exception...\n";
		throw float(42);
	} catch( ... ) {
		std::cout << " the integer exception was caught" << '\n';
	}

	try {
		std::cout << "Creating a vector of size 5... \n";
		std::vector<int> v(5);
		std::cout << "Accessing the 11th element of the vector...\n";
		v.at(10);  // the at() function will check the range.
	} catch( std::exception& e) {
		std::cout << " a standard exception was caught, with message '" << e.what() << "'\n";
	}
}

void keyword_bitand() {
	cout << "\nKeyword bitand\n";
	int x = 1;
	int y = 2;
	cout << x << " bitand " << y << " = " << (x bitand y) << endl;
	cout << x << " & " << y << " = " << (x & y) << endl;
	y = 3;
	cout << x << " bitand " << y << " = " << (x bitand y) << endl;
	cout << x << " & " << y << " = " << (x & y) << endl;
}

void keyword_bitor() {
	cout << "\nKeyword bitor\n";
	int x = 1;
	int y = 2;
	cout << x << " bitor " << y << " = " << (x bitor y) << endl;
	cout << x << " | " << y << " = " << (x | y) << endl;
	y = 3;
	cout << x << " bitor " << y << " = " << (x bitor y) << endl;
	cout << x << " | " << y << " = " << (x | y) << endl;
}

void keyword_type() {
	cout << "\nKeyword type\n";
	char c = 'f';
	cout << c << endl;
	cout << "Is char fundamental? " << is_fundamental<char>::value << endl;
	cout << "Is int fundamental? " << is_fundamental<int>::value << endl;
	cout << "Is bool fundamental? " << is_fundamental<bool>::value << endl;
	cout << "Is float fundamental? " << is_fundamental<float>::value << endl;
	cout << "Is string fundamental? " << is_fundamental<string>::value << endl;
	cout << "Is int[] fundamental? " << is_fundamental<int[]>::value << endl;
	cout << "Is void fundamental? " << is_fundamental<void>::value << endl;
	cout << "Is nullptr_t fundamental? " << is_fundamental<nullptr_t>::value << endl;
	cout << "Is char16_t fundamental? " << is_fundamental<char16_t>::value << endl;
	cout << "Is wchar_t fundamental? " << is_fundamental<wchar_t>::value << endl;

	char16_t c16 = 65;
	cout << c16 << endl;

	wchar_t cw = 65;
	cout << cw << endl;
	cout << sizeof(wchar_t) << endl;
	cout << sizeof(char32_t) << endl;
}

void keyword_class() {
	cout << "\nKeyword class\n";
	Person yannis("Yannis");
	yannis.setLastname("Thomias");
	yannis.pet.setName("Bob");
	cout << yannis.getFirstname() << " " << yannis.getLastname() << ": " << yannis.pet.getName() << endl;

	Person *jl = NULL;
	jl = new Person("Jean-Louis");
	jl->setLastname("Guenego");
	jl->pet.setName("Chipie");
	cout << jl->getFirstname() << " " << jl->getLastname() << ": " << jl->pet.getName() << endl;
	delete jl;
}

Person::Person(string firstname) {
	this->firstname = firstname;
};

Person::~Person() {
	cout << "Deleting " << this->firstname << endl;
};

void Person::setLastname(string l) {
	this->lastname = l;
};

string Person::getLastname() {
	return this->lastname;
}

string Person::getFirstname() {
	return this->firstname;
}

int Person::getAge() {
	return this->age;
}

Animal::Animal() {
};

Animal::~Animal() {
	cout << "Deleting " << this->name << endl;
};

void Animal::setName(string name) {
	this->name = name;
};

string Animal::getName() {
	return this->name;
};

void keyword_compl() {
	cout << "\nKeyword compl\n";
	int x = 0;
	cout << "compl " << x << " = " << compl x << endl;
	cout << "~" << x << " = " << ~x << endl;
	x = 3;
	cout << "compl " << x << " = " << compl x << endl;
	cout << "~" << x << " = " << ~x << endl;
}

void keyword_const() {
	cout << "\nKeyword const\n";
	int const x = 4;
	cout << "x=" << x << endl;

	int j[2] = {4, 8};
	const int* i = j;
	cout << *i << endl;
//	(*i)++;
//	i++;
	cout << *i << endl;
}

constexpr int multiply(int a, int b) {
	return a * b;
}

void keyword_constexpr(int a) {
	cout << "\nKeyword constexpr\n";
	int x = multiply(10, 10);
	cout << x << endl;

	int array[a];
	array[0] = 3;
	cout << array[0] << endl;
	cout << sizeof(array) << endl;
}

void keyword_const_cast() {
	cout << "\nKeyword const_cast\n";

	cout << sizeof(short) << endl;
	cout << sizeof(int) << endl;

	int i = 1 << 17;
	cout << i << endl;
	i--;
	short j = i;
	cout << j << endl;

	i = 1 << 17;
	unsigned int ui = -i;
	cout << ui << endl;
	int k = ui;
	cout << k << endl;

	bool b = k;
	cout << b << endl;
	bool nb = 0;
	cout << nb << endl;

	float a = 2.3;
	i = a;
	cout << i << endl;
	a = 2.9;
	i = a;
	cout << i << endl;

	a = -3.2;
	ui = a;
	cout << ui << endl;
	ui += 3;
	cout << ui << endl;
}

class Dummy {
	public:
		int i,j;
};

class Addition {
		int x,y;
	public:
		Addition (int a, int b) { x=a; y=b; }
		int result() { return x+y;}
};

void keyword_cast1() {
	cout << "\nKeyword cast1\n";

	cout << "int=" << sizeof(int) << endl;
	cout << "double=" << sizeof(double) << endl;
	cout << "float=" << sizeof(float) << endl;

	Dummy d;
	d.i = 2;
	d.j = 3;
	Addition * padd;
	padd = (Addition*) &d;
	cout << padd->result();
}

void print (char * str) {
	//str[0] = 'a';
	cout << str << endl;
}

void keyword_cast2() {
	cout << "\nKeyword cast2\n";
	char *tmp = "sample text";
	char buffer[64];
	snprintf(buffer, 64, tmp);
	const char *c = buffer;
	print(const_cast<char *>(c));
}

int& foo(int& a) {
	a = a * 2;
	return a;
}

void keyword_refto() {
	cout << "\nKeyword refto\n";
	int a = 2;
	int& b = a;
	cout << "a=" << a << endl;
	cout << "b=" << b << endl;
	a = 8;
	cout << "a=" << a << endl;
	cout << "b=" << b << endl;
	b = 12;
	cout << "a=" << a << endl;
	cout << "b=" << b << endl;
	b = foo(a);
	cout << "a=" << a << endl;
	cout << "b=" << b << endl;
}

class A {
	public:
		int x = 1;
		A() {
			cout << "Construct A" << endl;
		}

		A(int x) {
			this->x = x;
			cout << "Construct A(" << x << ")" << endl;
		}

		A(const A& a) {
			cout << "Copy constructor A" << endl;
		}

		A& operator=(const A& a) {
			cout << "Copy assignment operator" << endl;
			this->x = a.x;
			printf("this->x=%d\n", this->x);
			return *this;
		}
};

class B {
};

void keyword_default() {
	cout << "\nKeyword default\n";
	print("1");
	A a(2);
	print("2");
	A b = a;
	print("3");
	A& c = a;
	print("4");
	A d;
	print("5");
	d = a;
	printf("d.x=%d\n", d.x);
}

class P1 {
	public:
		virtual void say_hello() = 0;
};

class A1 : public virtual P1 {
		void say_hello() override {
			cout << "A says Hello!" << endl;
		};
};

class B1 : P1 {
	public:
		void say_hello() override {
			cout << "B says Hello!" << endl;
		};
};

void keyword_abstract() {
	cout << "\nkeyword_abstract\n";
	A1 a;
	//a.say_hello();
	B1 b;
	b.say_hello();
	P1& p = a;
	p.say_hello();
	//P1 p;
	//p.say_hello();
}

class A2 {
	public:
		virtual ~A2() {};
};

class B2 : public A2 {
	public:
		int x = 2;
};

class C2 {
};

void keyword_cast() {
	cout << "\nkeyword_cast\n";
	B2 *b = new B2();
	//A2 *a = static_cast<A2*>(b);
	A2 *a2 = new A2();
	B2 *b2 = dynamic_cast<B2*>(a2);
	if (b2) {
		print("Cast possible.");
	} else {
		print("Cast not possible.");
	}

	float a3 = 3.14;
	int b3 = (int) a3;
	printf("a3=%f, b3=%d\n", a3, b3);
	printf("int=%d, float=%d\n", sizeof(int), sizeof(float));
	int& c3 = reinterpret_cast<int&>(a3);
	printf("a3=%f, c3=%d\n", a3, c3);

	int a4 = 1;
	float& c4 = reinterpret_cast<float&>(a4);
	printf("a4=%d, c4=%e\n", a4, c4);
}

enum E {
	LUNDI,
	MARDI,
	MERCREDI
};

void keyword_enum() {
	cout << "\nkeyword_enum\n";
	cout << MARDI << endl;
}

class A3 {
	public:
		/*explicit*/ A3(int x) {};
};

void bad_print_A3(A3 a) {
	print("bad_print_A3");
}

void keyword_explicit() {
	cout << "\nkeyword_explicit\n";
	bad_print_A3(4);
}

template<class F, class A>
void Fwd(F f, A a) {
    f(a);
}

void g(int* i) {
    std::cout << "Function g called\n";
}

void keyword_nullptr() {
	cout << "\nkeyword_nullptr\n";
	g(NULL);			// Fine
	g(0);				// Fine
	Fwd(g, nullptr);	// Fine
//	Fwd(g, NULL);		// ERROR: No function g(int)
}

class A4 {
	protected:
		void say_hello() {
			print("Coucou");
		};
};

class B4 : A4 {
	public:
		void b_say_hello() {
			say_hello();
		};
};

void keyword_protected() {
	cout << "\nkeyword_protected\n";
	A4 a;
//	a.say_hello();
	B4 b;
	b.b_say_hello();
}

class A5 {
	static int count;
	public:
		A5() {
			count++;
		};

		static int getCount() {
			return count;
		};
};

int A5::count = 0;

void keyword_static() {
	const int b = 3;
	cout << "\nkeyword_static\n";
	for (int i = 0; i < 10; i++) {
		A5 a;
		printf("count=%d\n", A5::getCount());
		static_assert(b < 5, "b must be < 5.");
	}
}

template<class T>
class my_list {

	class node {
		public:
			node(T t) {
				this->data = t;
			};
			node *next = nullptr;
			T data;
	};

	node *head = nullptr;
	string buffer;

	public:
		void add(T t);
		T& get(int i);
		void remove(int i);
		string& to_string();
};

template<class T>
void my_list<T>::add(T t) {
	node *last = new node(t);
	if (this->head == nullptr) {
		this->head = last;
		return;
	}
	node *n = this->head;
	while (n->next != nullptr) {
		n = n->next;
	}
	n->next = last;
}

template<class T>
T& my_list<T>::get(int index) {
	node *n = this->head;
	for (int i = 0; i < index; i++) {
		n = n->next;
	}
	return n->data;
}

template<class T>
void my_list<T>::remove(int index) {
	node *n = this->head;
	node *previous = nullptr;
	for (int i = 0; i < index; i++) {
		previous = n;
		n = n->next;
	}
	previous->next = n->next;
	delete n;
}

template<class T>
string& my_list<T>::to_string() {
	buffer = "[";
	bool is_first = true;
	node *n = head;
	while (n != nullptr) {
		if (!is_first) {
			buffer += ", ";
		}
		is_first = false;
		char buf[512];
		snprintf(buf, 512, "%d", (int) n->data);
		buffer += buf;
//		buffer += std::to_string(n->data);
		n = n->next;
	}
	buffer += "]";
	return buffer;
}

template<class T>
ostream& operator<<(ostream& cout, my_list<T> obj)
{
	cout << obj.to_string();
	return cout;
}

void keyword_template() {
	cout << "\nkeyword_template\n";
	my_list<int> li;
	li.add(3);
	li.add(5);
	li.add(7);
	cout << li << endl;
	printf("li[2]=%d\n", li.get(2));
	li.remove(1);
	cout << li << endl;
}


thread_local unsigned int rage = 1;
std::mutex cout_mutex;

void increase_rage(const std::string& thread_name) {
    ++rage;
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Rage counter for " << thread_name << ": " << rage << '\n';
}

void keyword_thread_local() {
	cout << "\nkeyword_thread_local\n";
	std::thread a(increase_rage, "a"), b(increase_rage, "b");

	{
		std::lock_guard<std::mutex> lock(cout_mutex);
		std::cout << "Rage counter for main: " << rage << '\n';
	}

	a.join();
	b.join();
}