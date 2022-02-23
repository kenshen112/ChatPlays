#include <iostream>

enum ABS {L_AXIS, R_AXIS, DPAD_AXIS, L2, R2};
enum Buttons {A, B, X, Y, START, SELECT, L1, R1, L3, R3, EXIT};

class Controller
{

protected:
int controllerID;

public:
virtual bool AttachController() = 0;
virtual int PressButton(Buttons b) = 0;
virtual int ReleaseButton(Buttons b) = 0;
virtual void MoveABS(ABS abs, float axis) = 0;
virtual void ResetABS(ABS abs) = 0;
virtual void DisconnectController() = 0;
};