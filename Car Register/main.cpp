#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;

class Car
{
public:
	string plate;
	string owner_name;
	string owner_surname;
	Car ()
	{
		this->plate = "";
		this->owner_name =  "";
		this->owner_surname = "";
	}	
};

class Person
{
  public:	
  	string name;
  	string surname;
  	vector<string> persons_cars;
  	Person (){
  		this->name = "";
  		this->surname = "";
  	}
};


struct s_ort_people
{
    bool operator() (const Person& person1, const Person& person2)
    {
        if (person1.surname == person2.surname){
          return(person1.name < person2.name);
        }
        else {
          return (person1.surname < person2.surname);
        }
    }
};

struct s_ort_cars
{
    bool operator() (const Car& car1, const Car& car2)
    {
        return (car1.plate < car2.plate);
    }

};

class CCarList
{
  public:
    const string & RZ      ( void ) const { return cars_it[car_pos];}
    bool           AtEnd   ( void ) const { 	if ((int)cars_it.size() == car_pos) return true; 
    																				return false;}
    void           Next    ( void ) {car_pos++;}
  private:
    int car_pos;
  public:  
    CCarList(void) : car_pos(0){}
    vector<string> cars_it; 
};

class CPersonList
{
  public:
    const string & Name    ( void ) const { return dudes[dude_pos].name;}
    const string & Surname ( void ) const { return dudes[dude_pos].surname;}

    bool           AtEnd   ( void ) const { if ((int)dudes.size() == dude_pos) return true;
																				                                       return false;}
    void           Next    ( void ) {dude_pos++;}
  private:
  	int dude_pos;
  public:	  	
    CPersonList (void) : dude_pos (0) {}
	  vector<Person> dudes;
};



class CRegister
{
  public:
                CRegister  ( void ) {}
               ~CRegister  ( void ) {}
                CRegister  ( const CRegister & src ) = delete;
    CRegister & operator = ( const CRegister & src ) = delete;
    bool        AddCar     ( const string & rz,
                             const string & name,
                             const string & surname );
    bool        DelCar     ( const string & rz );
    bool        Transfer   ( const string & rz,
                             const string & nName,
                             const string & nSurname);
    CCarList    ListCars   ( const string & name,
                             const string & surname ) const;
    int         CountCars  ( const string & name,
                             const string & surname ) const;
    CPersonList ListPersons( void ) const {
    	CPersonList person_list;

		person_list.dudes = persons;

		return person_list;
    }
  private:
    vector<Person> persons;
    vector<Car> cars;
};

/*void show_db (vector<Car> car, vector<Person> person){

    cout << "--------------Cars------------" << endl;
    for (int i = 0; i < (int)car.size(); ++i){
        cout << car[i].plate << ' ' << car[i].owner_name << ' ' << car[i].owner_surname << endl;
    }
    cout << "--------------People------------" << endl;
    for (int k = 0; k < (int)person.size(); ++k){
        cout << person[k].name << ' ' << person[k].surname << endl;
        for (int y = 0; y < (int)person[k].persons_cars.size(); ++y)
        {
            cout << person[k].persons_cars[y] << endl;
        }
    }

}*/

bool CRegister :: AddCar (const string & rz, const string & name, const string & surname )
{	
	Car tmp_car;
		tmp_car.plate = rz;
		tmp_car.owner_name = name;
		tmp_car.owner_surname = surname;	

	Person tmp_person;
		tmp_person.name = name;
		tmp_person.surname = surname;
		tmp_person.persons_cars.push_back(rz);	

	auto p = lower_bound(persons.begin(), persons.end(), tmp_person, s_ort_people());
	auto c = lower_bound(cars.begin(), cars.end(), tmp_car, s_ort_cars());


	if (c != cars.end() && c->plate == rz) {return false;}
		
	cars.insert(c, tmp_car);

	if (p != persons.end() && p->name == name && p->surname == surname)
	{
		p->persons_cars.push_back(rz);
	}
	else
	{	
		persons.insert(p, tmp_person);
	}

	return true;
}


bool CRegister :: DelCar (const string & rz)
{
	Car tmp_car;
		tmp_car.plate = rz;

	if (cars.empty())
	{
		return false;
	}	

	auto c = lower_bound(cars.begin(), cars.end(), tmp_car, s_ort_cars());

	if (c == cars.end() || c->plate != rz) return false;

	Person tmp_person;
		tmp_person.name = c->owner_name;
		tmp_person.surname = c->owner_surname;

	auto p = lower_bound(persons.begin(), persons.end(), tmp_person, s_ort_people());
	
	for (int i = 0; i < (int)p->persons_cars.size(); ++i)
	{
		if (p->persons_cars[i] == rz)
		{
			p->persons_cars.erase(p->persons_cars.begin()+i);
		}
	}

	if (p->persons_cars.empty()) persons.erase(p); 

	cars.erase(c);
			
	return true;		
}


bool CRegister :: Transfer (const string & rz, const string & nName, const string & nSurname)
{
	Car tmp_car;
		tmp_car.plate = rz;

	Person tmp_person;
		tmp_person.name = nName;
		tmp_person.surname = nSurname;

	auto c = lower_bound(cars.begin(), cars.end(), tmp_car, s_ort_cars());

	if (c == cars.end() || c->plate != rz || (c->owner_name == nName && c->owner_surname == nSurname)) return false;
	
	Person del_person;
		del_person.name = c->owner_name;
		del_person.surname = c->owner_surname;
	
		c->owner_name = nName;
		c->owner_surname = nSurname;
	
	auto p = lower_bound(persons.begin(), persons.end(), tmp_person, s_ort_people());
	
	if (p != persons.end() && p->name == nName && p->surname == nSurname)
	{		
		p->persons_cars.push_back(rz);
	}
	else{
		tmp_person.persons_cars.push_back(rz);
		persons.insert(p,tmp_person);	
	}	

	auto dp = lower_bound(persons.begin(), persons.end(), del_person, s_ort_people());
	
  for (int i = 0; i < (int)dp->persons_cars.size(); ++i)
	{
		if (dp->persons_cars[i] == rz) dp->persons_cars.erase(dp->persons_cars.begin() + i); 
	}

	if (dp->persons_cars.empty()) persons.erase(dp); 

	return true;
}


CCarList  CRegister ::  ListCars   (const string & name, const string & surname ) const
{
	CCarList carlist;

	Person tmp_person;
		tmp_person.name = name;
		tmp_person.surname = surname;

	auto p = lower_bound(persons.begin(), persons.end(), tmp_person, s_ort_people());	
	
	if (p == persons.end() || (p->name != name && p->surname != surname))
	{
		return carlist;
	}	

	carlist.cars_it = p->persons_cars;

	return carlist;
}


int CRegister :: CountCars (const string & name, const string & surname) const
{
	Person tmp_person; 
		tmp_person.name = name;
		tmp_person.surname = surname;

	auto p = lower_bound(persons.begin(), persons.end(), tmp_person, s_ort_people()); 

	if (p == persons.end() || (p->name != name && p->surname != surname)) return 0;	

	return p->persons_cars.size();
}

bool checkPerson           ( CRegister    & r,
                             const string & name,
                             const string & surname,
                             vector<string> result )
{
  for ( CCarList l = r . ListCars ( name, surname ); ! l . AtEnd (); l . Next () )
  {
    auto pos = find ( result . begin (), result . end (), l . RZ () ); 
    if ( pos == result . end () )
      return false;
    result . erase ( pos );
  }
  return result . size () == 0;
}

int main ( void )
{
  CRegister b1;
  assert ( b1 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
  assert ( b1 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
  assert ( b1 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
  assert ( b1 . CountCars ( "John", "Hacker" ) == 1 );
  assert ( checkPerson ( b1, "John", "Hacker", { "ABC-32-22" } ) );
  assert ( b1 . Transfer ( "XYZ-11-22", "John", "Hacker" ) == true );
  assert ( b1 . AddCar ( "XYZ-99-88", "John", "Smith" ) == true );
  assert ( b1 . CountCars ( "John", "Smith" ) == 2 );
  assert ( checkPerson ( b1, "John", "Smith", { "ABC-12-34", "XYZ-99-88" } ) );
  assert ( b1 . CountCars ( "John", "Hacker" ) == 2 );
  assert ( checkPerson ( b1, "John", "Hacker", { "ABC-32-22", "XYZ-11-22" } ) );
  assert ( b1 . CountCars ( "Peter", "Smith" ) == 0 );
  assert ( checkPerson ( b1, "Peter", "Smith", {  } ) );
  assert ( b1 . Transfer ( "XYZ-11-22", "Jane", "Black" ) == true );
  assert ( b1 . CountCars ( "Jane", "Black" ) == 1 );
  assert ( checkPerson ( b1, "Jane", "Black", { "XYZ-11-22" } ) );
  assert ( b1 . CountCars ( "John", "Smith" ) == 2 );
  assert ( checkPerson ( b1, "John", "Smith", { "ABC-12-34", "XYZ-99-88" } ) );
  assert ( b1 . DelCar ( "XYZ-11-22" ) == true );
  assert ( b1 . CountCars ( "Jane", "Black" ) == 0 );
  assert ( checkPerson ( b1, "Jane", "Black", {  } ) );
  assert ( b1 . AddCar ( "XYZ-11-22", "George", "White" ) == true );
  CPersonList i1 = b1 . ListPersons ();
  assert ( ! i1 . AtEnd () && i1 . Surname () == "Hacker" && i1 . Name () == "John" );
  assert ( checkPerson ( b1, "John", "Hacker", { "ABC-32-22" } ) );
  i1 . Next ();
  assert ( ! i1 . AtEnd () && i1 . Surname () == "Smith" && i1 . Name () == "John" );
  assert ( checkPerson ( b1, "John", "Smith", { "ABC-12-34", "XYZ-99-88" } ) );
  i1 . Next ();
  assert ( ! i1 . AtEnd () && i1 . Surname () == "White" && i1 . Name () == "George" );
  assert ( checkPerson ( b1, "George", "White", { "XYZ-11-22" } ) );
  i1 . Next ();
  assert ( i1 . AtEnd () );

  CRegister b2;
  assert ( b2 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
  assert ( b2 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
  assert ( b2 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
  assert ( b2 . AddCar ( "XYZ-11-22", "Jane", "Black" ) == false );
  assert ( b2 . DelCar ( "AAA-11-11" ) == false );
  assert ( b2 . Transfer ( "BBB-99-99", "John", "Smith" ) == false );
  assert ( b2 . Transfer ( "ABC-12-34", "John", "Smith" ) == false );
  assert ( b2 . CountCars ( "George", "White" ) == 0 );
  assert ( checkPerson ( b2, "George", "White", {  } ) );
  CPersonList i2 = b2 . ListPersons ();
  assert ( ! i2 . AtEnd () && i2 . Surname () == "Hacker" && i2 . Name () == "John" );
  assert ( checkPerson ( b2, "John", "Hacker", { "ABC-32-22" } ) );
  i2 . Next ();
  assert ( ! i2 . AtEnd () && i2 . Surname () == "Smith" && i2 . Name () == "John" );
  assert ( checkPerson ( b2, "John", "Smith", { "ABC-12-34" } ) );
  i2 . Next ();
  assert ( ! i2 . AtEnd () && i2 . Surname () == "Smith" && i2 . Name () == "Peter" );
  assert ( checkPerson ( b2, "Peter", "Smith", { "XYZ-11-22" } ) );
  i2 . Next ();
  assert ( i2 . AtEnd () );

  return 0;
}