#pragma once
#include "Core.h"
#include <SDL/SDL_log.h>
#include <SDL/SDL_error.h>


// [---------------------------------------------]
// [------------------- ERROR -------------------]
#define CB_GetError(...)				SDL_GetError(__VA_ARGS__)
#define CB_ClearError(...)				SDL_ClearError(__VA_ARGS__)
#define CB_GetErrorMsg(...)				SDL_GetErrorMsg(__VA_ARGS__)
#define CB_SetError(...)				SDL_SetError(__VA_ARGS__)


// [---------------------------------------------]
// [-------------------- LOG --------------------]
#define CB_LogVerbose(...)				SDL_LogVerbose(__VA_ARGS__)
#define CB_LogDebug(...)				SDL_LogDebug(__VA_ARGS__)
#define CB_LogInfo(...)					SDL_LogInfo(__VA_ARGS__)
#define CB_LogWarn(...)					SDL_LogWarn(__VA_ARGS__)
#define CB_LogError(...)				SDL_LogError(__VA_ARGS__)
#define CB_LogCritical(...)				SDL_LogCritical(__VA_ARGS__)
#define CB_LogMessage(...)				SDL_LogMessage(__VA_ARGS__)

#define CB_GET_LOG_CATEGORY(...)		SDL_LogGetPriority(__VA_ARGS__)
#define CB_SET_LOG_CATEGORY(...)		SDL_LogSetPriority(__VA_ARGS__)

// CATEGORIES
// Test is enabled at the VERBOSE level
// The application category is enabled at the INFO level, 
// The assert category is enabled at the WARN level, 
// All other categories are enabled at the CRITICAL level.
#define	LOG_TEST						SDL_LOG_CATEGORY_TEST
#define	LOG_APP							SDL_LOG_CATEGORY_APPLICATION
#define	LOG_ASSERT						SDL_LOG_CATEGORY_ASSERT
#define	LOG_ERROR						SDL_LOG_CATEGORY_ERROR
#define	LOG_SYSTEM						SDL_LOG_CATEGORY_SYSTEM
#define	LOG_AUDIO						SDL_LOG_CATEGORY_AUDIO
#define	LOG_VIDEO						SDL_LOG_CATEGORY_VIDEO
#define	LOG_RENDER						SDL_LOG_CATEGORY_RENDER
#define	LOG_INPUT						SDL_LOG_CATEGORY_INPUT

// PRIORITIES
#define LOG_PRIORITY_VERBOSE			SDL_LOG_PRIORITY_VERBOSE
#define LOG_PRIORITY_DEBUG				SDL_LOG_PRIORITY_DEBUG
#define LOG_PRIORITY_INFO				SDL_LOG_PRIORITY_INFO
#define LOG_PRIORITY_WARN				SDL_LOG_PRIORITY_WARN
#define LOG_PRIORITY_ERROR				SDL_LOG_PRIORITY_ERROR
#define LOG_PRIORITY_CRITICAL			SDL_LOG_PRIORITY_CRITICAL