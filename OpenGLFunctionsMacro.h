#pragma once
#ifndef OPENGLFUNCTIONSMACRO_H_
#define OPENGLFUNCTIONSMACRO_H_

#if defined (_USE_QT_FRAMEWORK_)
#define glUseProgram funcs->glUseProgram
#define glBindVertexArray funcs->glBindVertexArray
#endif // _USE_QT_FRAMEWORK_

#endif // OPENGLFUNCTIONSMACRO_H_