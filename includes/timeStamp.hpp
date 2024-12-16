/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeStamp.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:11:05 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:11:06 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "utils_templates.hpp"
#include "libraries.hpp"
#define SPACE " "

class timeStamp
{

private:

	// Useless Coplian methods
	timeStamp( const timeStamp& copy );
	timeStamp& operator=( const timeStamp& right_operator );

public:

	timeStamp( void );
	~timeStamp();

	static string getTime( void );

};