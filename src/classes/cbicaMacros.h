/**
\file  cbicaMacros.h

\brief Contains common macros to be used

http://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or http://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#define cbicaEchoVersionMacro( N, R, SVN_R, SVN_FV ) \
{ \
  std::cout << std::endl << N << std::endl << \
  INDENT << " Release          : " << R << std::endl << \
  INDENT << " Svn Revision     : " << SVN_R << std::endl << \
  INDENT << " Svn File verions : " << SVN_FV << std::endl \
  << std::endl; \
}
