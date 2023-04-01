#ifndef ARMSTRONG_FIRMWARE_STATE_H
#define ARMSTRONG_FIRMWARE_STATE_H

#include <memory>

class State {
public:
	virtual ~State() = default;

	void start();
	void stop();
	bool isStarted() const;

	static void enterRecord();
	static void enterPlayback();
	static void enterI2C();

protected:
	virtual void onStart();
	virtual void onStop();

private:
	bool started = false;

	static std::unique_ptr<State> currentState;
	static void clearCurrent();
	static void startNew(State* state);

};

#endif //ARMSTRONG_FIRMWARE_STATE_H