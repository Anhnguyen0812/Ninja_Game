
#include "MainObject.h"
#include "windows.h"
int main(int a, int b)
{
	if (a > b)
		return b;
	else
		return a;
}
void MainObject::increase_coins()
{

	coins++;
}


MainObject::MainObject()
{
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_NONE;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.jump_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
	map_x_ = 0;
	map_y_ = 0;
	come_back_time_ = 0;
	jum_times = 0;
	on_land = false;
	coins = 0;
}

MainObject::~MainObject()
{

}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);

	if (ret == true) {
		width_frame_ = rect_.w / 8;
		height_frame_ = rect_.h;
	}

	return ret;

}

void MainObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		frame_clip_[0].x = 0;
		frame_clip_[0].y = 0;
		frame_clip_[0].w = width_frame_;
		frame_clip_[0].h = height_frame_;

		frame_clip_[1].x = width_frame_;
		frame_clip_[1].y = 0;
		frame_clip_[1].w = width_frame_;
		frame_clip_[1].h = height_frame_;

		frame_clip_[2].x = 2 * width_frame_;
		frame_clip_[2].y = 0;
		frame_clip_[2].w = width_frame_;
		frame_clip_[2].h = height_frame_;

		frame_clip_[3].x = 3 * width_frame_;
		frame_clip_[3].y = 0;
		frame_clip_[3].w = width_frame_;
		frame_clip_[3].h = height_frame_;

		frame_clip_[4].x = 4 * width_frame_;
		frame_clip_[4].y = 0;
		frame_clip_[4].w = width_frame_;
		frame_clip_[4].h = height_frame_;

		frame_clip_[5].x = 5 * width_frame_;
		frame_clip_[5].y = 0;
		frame_clip_[5].w = width_frame_;
		frame_clip_[5].h = height_frame_;

		frame_clip_[6].x = 6 * width_frame_;
		frame_clip_[6].y = 0;
		frame_clip_[6].w = width_frame_;
		frame_clip_[6].h = height_frame_;

		frame_clip_[7].x = 7 * width_frame_;
		frame_clip_[7].y = 0;
		frame_clip_[7].w = width_frame_;
		frame_clip_[7].h = height_frame_;

	}
}

void MainObject::Show(SDL_Renderer* des)
{
	UpdateImgPlayer(des);

	if (input_type_.left_ == 1
		|| input_type_.right_ == 1)
	{
		frame_++;
	}
	else
	{
		frame_ = 0;
	}

	if (frame_ >= 8)
		frame_ = 0;

	if (come_back_time_ == 0)
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		SDL_Rect* current_clip = &frame_clip_[frame_];

		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

		SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

	}
}

void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer* screen)
{
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
			UpdateImgPlayer(screen);
		}
		break;
		case SDLK_LEFT:
		{
			status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			UpdateImgPlayer(screen);
		}
		break;
		case SDLK_UP:
		{
			input_type_.jump_ = 1;
		}
		break;
		case SDLK_DOWN:
		{
			input_type_.down_ = 1;
		}
		break;
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{

			input_type_.right_ = 0;
		}
		break;

		case SDLK_LEFT:
		{
			input_type_.left_ = 0;
		}
		break;

		case SDLK_UP:
		{
			input_type_.jump_ = 0;
		}
		break;
		case SDLK_DOWN:
		{
			input_type_.down_ = 0;
		}
		}
	}
}

void MainObject::DoPlayer(Map& map_data)
{
	if (come_back_time_ == 0)
	{
		x_val_ = 0;
		y_val_ += Gravity_speed;

		if (y_val_ >= MAX_FALL_SPEED)
		{
			y_val_ = MAX_FALL_SPEED;
		}

		if (input_type_.left_ == 1) {
			x_val_ -= PLAYER_SPEED;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += PLAYER_SPEED;
		}
		if (on_land == true)
			jum_times = 0;

		if (input_type_.jump_ == 1)
		{
			if (on_land == true)
			{
				y_val_ = - PLAYER_JUMP;
			}
			else if (jum_times == 0) {
				y_val_ -= 7;
				jum_times = 1;
			}
			
			on_land = false;
			input_type_.jump_ = 0;
			
		}

		if (input_type_.down_ == 1)
		{
			y_val_ += 2;
		}

		CheckToMap(map_data);
		CenterEntityOnMap(map_data);
	}

	if (come_back_time_ > 0) {
		come_back_time_--;

		if (come_back_time_ == 0)
		{
			on_land = false;
			if (x_pos_ > 256)
			{
				x_pos_ -= 256;
			}
			else
			{
				x_pos_ = 0;
			}
		
			y_pos_ = 0;
			x_val_ = 0;
			y_val_ = 0;
		}
	}
}

void MainObject::CenterEntityOnMap(Map& map_data)
{
	map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);
	if (map_data.start_x_ < 0)
	{
		map_data.start_x_ = 0;
	}
	else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
	{
		map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
	}

	map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);
	if (map_data.start_y_ < 0)
	{
		map_data.start_y_ = 0;
	}
	else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
	{
		map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
	}
}

void MainObject::CheckToMap(Map& map_data)
{
	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;

	// check horizontal
	int height_min = min(height_frame_, TILE_SIZE);

	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;
	y1 = (y_pos_  + 1) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 1) / TILE_SIZE;


	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		/* v1 y1x1
		   v2 y1x2
		   v3 y2x1
		   v4 y2x2
		*/
		
		if (x_val_ > 0) // moving to right
		{
			int v2 = map_data.tile[y1][x2];
			int v4 = map_data.tile[y2][x2];
			if (v2 == 4 || v4 == 4)
			{

				if (v2 == 4)
				{
					map_data.tile[y1][x2] = 0;
					increase_coins();
				}
				int v4 = map_data.tile[y2][x2];
				if (v4 == 4)
				{
					map_data.tile[y2][x2] = 0;
					increase_coins();
				}

			}
			else
			{
				if (v2 != BLANK_TILE || v4 != BLANK_TILE)
				{
					x_pos_ = x2 * TILE_SIZE;
					x_pos_ -= width_frame_ + 1;
					x_val_ = 0;
				}

			}
		}
		else if (x_val_ < 0)
		{
			int v1 = map_data.tile[y1][x1];
			int v3 = map_data.tile[y2][x1];
			if (v1 == 4 || v3 == 4)
			{
				int v1 = map_data.tile[y1][x1];
				if (v1 == 4)
				{
					map_data.tile[y1][x1] = 0;
					increase_coins();
				}
				int v3 = map_data.tile[y2][x1];
				if (v3 == 4)
				{
					map_data.tile[y2][x1] = 0;
					increase_coins();
				}
			}
			else
			{
			
				if (v1 != BLANK_TILE || v3 != BLANK_TILE)
				{
					x_pos_ = (x1 + 1) * TILE_SIZE;
					x_val_ = 0;
				}
			}
		}
	}



	// vertical

	int width_min = min(width_frame_, TILE_SIZE);
	x1 = (x_pos_)/ TILE_SIZE;
	x2 = (x_pos_ + width_min - 1) / TILE_SIZE;
	y1 = (y_pos_ + y_val_ + 1) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

	

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{



		if (y_val_ > 0)
		{
			int v3 = map_data.tile[y2][x1];
			int v4 = map_data.tile[y2][x2];
			if (v3 == 4 || v4 == 4)
			{
				int v3 = map_data.tile[y2][x1];
				if (v3 == 4)
				{
					map_data.tile[y2][x1] = 0;
					increase_coins();
				}
				int v4 = map_data.tile[y2][x2];
				if (v4 == 4)
				{
					map_data.tile[y2][x2] = 0;
					increase_coins();
				}
			}
			else
			{
				if (v3 != BLANK_TILE || v4 != BLANK_TILE)
				{
					y_pos_ = (y2)*TILE_SIZE;
					y_pos_ -= (height_frame_ + 1);
					y_val_ = 0;
					on_land = true;
					if (status_ == WALK_NONE)
					{
						status_ = WALK_RIGHT;
					}
				}
			}
		}
		else if (y_val_ < 0)
		{
			int v1 = map_data.tile[y1][x1];
			int v2 = map_data.tile[y1][x2];
			if (v1 == 4 || v2 == 4)
			{
				int v1 = map_data.tile[y1][x1];
				if (v1 == 4)
				{
					map_data.tile[y1][x1] = 0;
					increase_coins();
				}
				int v2 = map_data.tile[y1][x2];
				if (v2 == 4)
				{
					map_data.tile[y1][x2] = 0;
					increase_coins();
				}
			}
				else
				{
					if (v1 != BLANK_TILE || v2 != BLANK_TILE)
					{
						y_pos_ = (y1 + 1) * TILE_SIZE;
						y_val_ = 0;
					}
				}

			}
		}

	

	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ < 0)
	{
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}

	if (y_pos_ > map_data.max_y_)
	{
		come_back_time_ = 50;
	}
}

void MainObject::UpdateImgPlayer(SDL_Renderer* des)
{
	if (on_land == true)
	{
		if (status_ == WALK_LEFT)
		{
			LoadImg("img//player sprite//player_left.png", des);
		}
		else if (status_ == WALK_RIGHT)
		{
			LoadImg("img//player sprite//player_right.png", des);
		}
	}
	else
	{
		if (status_ == WALK_LEFT)
		{
			LoadImg("img//jum_left.png", des);
		}
		else
		{
			LoadImg("img//jum_right.png", des);
		}

	}
}