#ifndef ARMSTRONG_FIRMWARE_STATE_H
#define ARMSTRONG_FIRMWARE_STATE_H

class State {
public:
	virtual ~State() = default;

	void start();
	void stop();
	[[nodiscard]] bool isStarted() const;

protected:
	virtual void onStart(){};

	virtual void onStop(){};

private:
	bool started = false;

};

#endif //ARMSTRONG_FIRMWARE_STATE_H