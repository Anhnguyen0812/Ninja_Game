
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include "CommonFunction.h"
#include "BaseObject.h"

#define Gravity_speed 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 4
#define PLAYER_JUMP 19



class MainObject : public BaseObject
{
public:
	MainObject();
	~MainObject();
	
	enum Walktype
	{
		WALK_NONE = 0,
		WALK_RIGHT = 1,	
		WALK_LEFT = 2,
	};

	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandelInputAction(SDL_Event events, SDL_Renderer* screen);
	void set_clips();
	void DoPlayer(Map& map_data);
	void CheckToMap(Map& map_data);
	void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; };
	void CenterEntityOnMap(Map& map_data);
	void UpdateImgPlayer(SDL_Renderer* des);
	void increase_coins() ;
	int return_coins() { return coins; };

private:
	float x_val_;
	float y_val_;

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8];
	
	Input input_type_;
	int frame_;
	int status_;
	bool on_land;
	int map_x_;
	int map_y_;
	int jum_times;
	int come_back_time_;
	int coins;
};



#endif