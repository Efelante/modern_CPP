#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

bool operator< (const Date &lhs, const Date &rhs){
	auto lDate = make_tuple(lhs.year, lhs.month, lhs.day);
	auto rDate = make_tuple(rhs.year, rhs.month, rhs.day);
	return lDate < rDate;
}

bool operator< (const Time &lhs, const Time &rhs){
	auto lTime = make_tuple(lhs.hours, lhs.minutes);
	auto rTime = make_tuple(rhs.hours, rhs.minutes);
	return lTime < rTime;
}

bool operator== (const Date &lhs, const Date &rhs){
	auto lDate = make_tuple(lhs.year, lhs.month, lhs.day);
	auto rDate = make_tuple(rhs.year, rhs.month, rhs.day);
	return lDate == rDate;
}

bool operator== (const Time &lhs, const Time &rhs){
	auto lTime = make_tuple(lhs.hours, lhs.minutes);
	auto rTime = make_tuple(rhs.hours, rhs.minutes);
	return lTime == rTime;
}

ostream& operator<< (ostream &os, const Date &date)
{
	os << date.year << "-" 
		<< date.month << "-"
		<< date.day << "-";
	return os;
}

ostream& operator<< (ostream &os, const Time &time)
{
	os << time.hours << "h " 
		<< time.minutes << "m";
	return os;
}

istream& operator>>(istream &stream, Date &date)
{
	int year;
	int month;
	int day;
	stream >> year;
	stream.ignore(1);
	stream >> month;
	stream.ignore(1);
	stream >> day;
	date = Date{year, month, day};
	return stream;
}

istream& operator>>(istream &stream, Time &time)
{
	int hours;
	int minutes;
	stream >> hours;
	stream.ignore(1);
	stream >> minutes;
	time = Time{hours, minutes};
	return stream;
}

#define UPDATE_FIELD(ticket, field, values) {\
	map<string, string>::const_iterator it; 	\
	it = values.find(#field); 				\
	if (it != values.end()) { 				\
		istringstream is(it->second); 	\
		is >> ticket.field; 				\
	}										\
}

void TestUpdate() {
  AirlineTicket t;
  t.price = 0;

  const map<string, string> updates1 = {
    {"departure_date", "2018-2-28"},
    {"departure_time", "17:40"},
  };
  UPDATE_FIELD(t, departure_date, updates1);
  UPDATE_FIELD(t, departure_time, updates1);
  UPDATE_FIELD(t, price, updates1);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 0);

  const map<string, string> updates2 = {
    {"price", "12550"},
    {"arrival_time", "20:33"},
  };
  UPDATE_FIELD(t, departure_date, updates2);
  UPDATE_FIELD(t, departure_time, updates2);
  UPDATE_FIELD(t, arrival_time, updates2);
  UPDATE_FIELD(t, price, updates2);

  // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
  // значения этих полей не должны измениться
  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 12550);
  ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestUpdate);
}
