#pragma once

#include <exception>
#include <string>

class SDLInitializationErrorException : public std::exception
{
public:

	SDLInitializationErrorException(const std::string& sdlError) :
		mMsg("SDL could not initialize! SDL_Error: %s\n" + sdlError)
	{}

	const char* what() const override
	{
		return mMsg.c_str();
	}

private:
	std::string mMsg;
};


class SDLWindowCreationErrorException : public std::exception
{
public:

	SDLWindowCreationErrorException(const std::string& sdlError) :
		mMsg("Window could not be created! SDL_Error: %s\n" + sdlError)
	{}

	const char* what() const override
	{
		return mMsg.c_str();
	}

private:
	std::string mMsg;
};


class SDLRendererCreationErrorException : public std::exception
{
public:

	SDLRendererCreationErrorException(const std::string& sdlError) :
		mMsg("Renderer could not be created! SDL_Error: %s\n" + sdlError)
	{}

	const char* what() const override
	{
		return mMsg.c_str();
	}

private:
	std::string mMsg;
};


class SDLImageInitializationErrorException : public std::exception
{
public:

	SDLImageInitializationErrorException(const std::string& sdlError) :
		mMsg("SDL_image could not initialize! SDL_Error: %s\n" + sdlError)
	{}

	const char* what() const override
	{
		return mMsg.c_str();
	}

private:
	std::string mMsg;
};


class SDLImageLoadErrorException : public std::exception
{
public:

	SDLImageLoadErrorException(const std::string& imagePath, const std::string& sdlError) :
		mMsg("Unable to load image " + imagePath + "! SDL_Image error: " + sdlError)
	{}

	const char* what() const override
	{
		return mMsg.c_str();
	}

private:
	std::string mMsg;
};


class SDLTextureCreationErrorException : public std::exception
{
public:

	SDLTextureCreationErrorException(const std::string& imagePath, const std::string& sdlError) :
		mMsg("Unable to create texture from " + imagePath + "! SDL error: " + sdlError)
	{}

	const char* what() const override
	{
		return mMsg.c_str();
	}

private:
	std::string mMsg;
};