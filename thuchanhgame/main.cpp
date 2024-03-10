/* Mã nguồn này được bản quyền bởi Lazy Foo' Productions 2004-2024
   và không được phân phối lại mà không có sự cho phép bằng văn bản. */

// Sử dụng SDL và standard IO
#include <SDL.h>
#include <stdio.h>

// Hằng số kích thước màn hình
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Khởi tạo SDL và tạo cửa sổ
bool init();

// Tải media
bool loadMedia();

// Giải phóng media và đóng SDL
void close();

// Cửa sổ mà chúng ta sẽ vẽ lên
SDL_Window* gWindow = NULL;

// Bề mặt được chứa bởi cửa sổ
SDL_Surface* gScreenSurface = NULL;

// Hình ảnh chúng ta sẽ tải và hiển thị trên màn hình
SDL_Surface* gHelloWorld = NULL;

bool init()
{
	// Cờ khởi tạo
	bool success = true;

	// Khởi tạo SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL không thể khởi tạo! SDL_Lỗi: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		// Tạo cửa sổ
		gWindow = SDL_CreateWindow( "Hướng dẫn SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Cửa sổ không thể tạo! SDL_Lỗi: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			// Lấy bề mặt của cửa sổ
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

bool loadMedia()
{
	// Cờ tải thành công
	bool success = true;

	// Tải hình ảnh
	gHelloWorld = SDL_LoadBMP( "bg2 (1).png" );
	if( gHelloWorld == NULL )
	{
		printf( "Không thể tải hình ảnh %s! SDL Lỗi: %s\n", "bg2 (1).png", SDL_GetError() );
		success = false;
	}

	return success;
}

void close()
{
	// Giải phóng bề mặt
	SDL_FreeSurface( gHelloWorld );
	gHelloWorld = NULL;

	// Hủy cửa sổ
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	// Kết thúc các hệ thống SDL
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	// Khởi động SDL và tạo cửa sổ
	if( !init() )
	{
		printf( "Khởi động thất bại!\n" );
	}
	else
	{
		// Tải media
		if( !loadMedia() )
		{
			printf( "Tải media thất bại!\n" );
		}
		else
		{
			// Áp dụng hình ảnh
			SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );

			// Cập nhật bề mặt
			SDL_UpdateWindowSurface( gWindow );

            // Hack để giữ cửa sổ mở
            SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
		}
	}

	// Giải phóng tài nguyên và đóng SDL
	close();

	return 0;
}
