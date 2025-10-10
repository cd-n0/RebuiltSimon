#ifndef INPUT_H_
#define INPUT_H_

void IN_ducktapUp(void);
void IN_ducktapDown(void);
void IN_leftUp(void);
void IN_leftDown(void);
void IN_rightUp(void);
void IN_rightDown(void);

extern kbutton_t in_ducktap;
extern kbutton_t in_left;
extern kbutton_t in_right;

#endif /* INPUT_H_ */