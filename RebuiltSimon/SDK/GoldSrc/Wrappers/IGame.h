#ifndef IGAME_H_
#define IGAME_H_

#include "RebuiltSimon/pch.h"
/* https://github.com/gmh5225/GameEngine-reGoldSrc/blob/7a8ed98263b9db9078ff2cfd9a81a95930ce691f/engine/IGame.h#L7 */
#if 0
typedef struct IGame_s
{
	void*	(*IGame_destructor)(struct IGame_s *thisptr);
	bool	(*Init)(struct IGame_s *thisptr, void* pvInstance);
	bool	(*Shutdown)(struct IGame_s *thisptr);
	bool	(*CreateGameWindow)(struct IGame_s *thisptr);
	void	(*SleepUntilInput)(struct IGame_s *thisptr, int time);
	void*	(*GetMainWindow)(struct IGame_s *thisptr);
	void**	(*GetMainWindowAddress)(struct IGame_s *thisptr);
	void	(*SetWindowXY)(struct IGame_s *thisptr, int x, int y);
	void	(*SetWindowSize)(struct IGame_s *thisptr, int w, int h);
	void	(*GetWindowRect)(struct IGame_s *thisptr, int* x, int* y, int* w, int* h);
	bool	(*IsActiveApp)(struct IGame_s *thisptr);
	bool	(*IsMultiplayer)(struct IGame_s *thisptr);
	void	(*PlayStartupVideos)(struct IGame_s *thisptr);
	void	(*PlayAVIAndWait)(struct IGame_s *thisptr, const char* fileName);
	void	(*SetCursorVisible)(struct IGame_s *thisptr, bool bState);
} IGame;
#else
/* Can't do the above (__thiscall), and I don't want to use inline asm */
HWND GetMainWindow(void* obj);
#endif

#endif /* IGAME_H_ */