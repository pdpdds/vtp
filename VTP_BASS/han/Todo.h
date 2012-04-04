// defines several macros making it possible to insert "super to-do" messages of the form
// #pragma TODO(message)
// which expand into strings that you can double-click on (in VC++'s build output window) to jump
// to the appropriate line

#if !defined(_Todo_H_Defined_)
#define _Todo_H_Defined_

#define LINE1(x) #x
#define LINE(x) LINE1(x)
#define TODO(msg) message ( __FILE__ "(" LINE(__LINE__)  "): [TODO] " msg )
#define NOTE(msg) message ( __FILE__ "(" LINE(__LINE__)  "): [NOTE] " msg )

#endif // !defined(_Todo_H_Defined_)


