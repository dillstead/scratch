/*
  Copyright 2005 Allen B. Downey

  This file contains an example program from The Little Book of
  Semaphores, available from Green Tea Press, greenteapress.com

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, see http://www.gnu.org/licenses/gpl.html
  or write to the Free Software Foundation, Inc., 51 Franklin St, 
  Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef pthread_mutex_t Mutex ;

Mutex * make_mutex ()
{
    Mutex * mutex = check_malloc ( sizeof ( Mutex ));
    int n = pthread_mutex_init ( mutex , NULL );
    if ( n != 0) perror_exit (" make_lock failed ");
    return mutex ;
}

void mutex_lock ( Mutex * mutex )
{
    int n = pthread_mutex_lock ( mutex );
    if ( n != 0) perror_exit (" lock failed ");
}

void mutex_unlock ( Mutex * mutex )
{
    int n = pthread_mutex_unlock ( mutex );
    if ( n != 0) perror_exit (" unlock failed ");
}

typedef pthread_cond_t Cond ;

Cond * make_cond ()
{
    Cond * cond = check_malloc ( sizeof ( Cond ));
    int n = pthread_cond_init ( cond , NULL );
    if (n != 0) perror_exit (" make_cond failed ");
    return cond ;
}

void cond_wait ( Cond * cond , Mutex * mutex )
{
    int n = pthread_cond_wait ( cond , mutex );
    if (n != 0) perror_exit (" cond_wait failed ");
}

void cond_signal ( Cond * cond )
{
    int n = pthread_cond_signal ( cond );
    if (n != 0) perror_exit (" cond_signal failed ");
}
