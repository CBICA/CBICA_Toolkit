/*!
 * \file  sbiaMacros.h
 * \brief Common macros.
 *
 * Copyright (c) 2011 University of Pennsylvania. All rights reserved.
 * See COPYING file or https://www.rad.upenn.edu/sbia/software/license.html.
 *
 * Contact: SBIA Group <sbia-software at uphs.upenn.edu>
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
