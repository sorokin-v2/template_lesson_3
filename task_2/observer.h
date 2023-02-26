#pragma once
#include <vector>
#include <memory>
#include <iostream>

class Observer {
public:
	virtual ~Observer() = default;
	virtual void showMessage(const std::string& message) const = 0;
};


class Observed {
public:

	Observed() = default;

	void AddWarningObserver(const Observer* observer) {
		warningObservers_.push_back(observer);
	}

	void AddErrorObserver(const Observer* observer) {
		errorObservers_.push_back(observer);
	}

	void AddFatalErrorObserver(const Observer* observer) {
		fatalErrorObservers_.push_back(observer);
	}

	void RemoveWarningObserver(const Observer* observer) {
		auto it = std::remove(warningObservers_.begin(), warningObservers_.end(), observer);
		warningObservers_.erase(it, warningObservers_.end());
	}

	void RemoveErrorObserver(const Observer* observer) {
		auto it = std::remove(errorObservers_.begin(), errorObservers_.end(), observer);
		errorObservers_.erase(it, errorObservers_.end());
	}

	void RemoveFatalErrorObserver(const Observer* observer) {
		auto it = std::remove(fatalErrorObservers_.begin(), fatalErrorObservers_.end(), observer);
		fatalErrorObservers_.erase(it, fatalErrorObservers_.end());
	}

	void warning(const std::string& message) const {
		for (auto observer : warningObservers_) {
			observer->showMessage(message);
		}
	}

	void error(const std::string& message) const {
		for (auto observer : errorObservers_) {
			observer->showMessage(message);
		}
	}

	void fatalError(const std::string& message) const {
		for (auto observer : fatalErrorObservers_) {
			observer->showMessage(message);
		}
	}

private:

	std::vector<const Observer*> warningObservers_;
	std::vector<const Observer*> errorObservers_;
	std::vector<const Observer*> fatalErrorObservers_;
};


class WarningObserver : public Observer {
public:
	WarningObserver() = delete;
	WarningObserver(Observed* subj) :subj_(subj) {
		subj_->AddWarningObserver(this);
	}
	~WarningObserver() override { subj_->RemoveWarningObserver(this); };

	void showMessage(const std::string& message) const override {
		std::cout << "Warning message: " << message << std::endl;
	}

private:
	Observed* subj_;
};

class ErrorObserver : public Observer {
public:
	ErrorObserver() = delete;
	ErrorObserver(Observed* subj) :subj_(subj) {
		subj_->AddErrorObserver(this);
	}
	~ErrorObserver() override { subj_->RemoveErrorObserver(this); };

	void showMessage(const std::string& message) const override {
		std::cout << "Error message: " << message << std::endl;
	}
private:
	Observed* subj_;
};

class FatalErrorObserver : public Observer {
public:
	FatalErrorObserver() = delete;
	FatalErrorObserver(Observed* subj) :subj_(subj) {
		subj_->AddFatalErrorObserver(this);
	}
	~FatalErrorObserver() override { subj_->RemoveFatalErrorObserver(this); };

	void showMessage(const std::string& message) const override {
		std::cout << "Fatal error message: " << message << std::endl;
	}
private:
	Observed* subj_;
};


