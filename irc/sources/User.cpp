#include "User.hpp"

User::User(int id) {
	this->id = id;
	this->authenticated = false;
	this->registered = false;
}

std::string User::get_nickname() const {
	return this->nickname;
}

std::string User::get_username() const {
	return this->username;
}

bool User::is_authenticated() const {
	return this->authenticated;
}

void User::set_authenticated(bool authenticated) {
	this->authenticated = authenticated;
} 

bool User::is_registered() const {
	return this->registered;
}

void User::set_registered(bool registered) {
	this->registered = registered;
}

void User::set_nickname(const std::string& nickname) {
	this->nickname = nickname;
}

void User::set_username(const std::string& username) {
	this->username = username;	
}

void User::add_response(Response* res) {
	responses.push(res);
}

bool User::has_response() const {
	return (this->responses.size());
}

Response* User::get_next_response() {
	Response* res = this->responses.front();
	this->responses.pop();
	return res;
}

std::string User::get_hostname() const {
	return this->hostname;
}

std::string User::get_realname() const {
	return this->realname;
}

void User::set_hostname(const std::string& hostname) {
	this->hostname = hostname;
}

void User::set_realname(const std::string& realname) {
	this->realname = realname;
}

bool User::is_username_set() const {
	if (this->username.size())
		return true;
	return false;
}

bool User::is_nickname_set() const {
	if (this->nickname.size())
		return true;
	return false;
}