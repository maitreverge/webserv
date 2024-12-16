/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_templates.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TryHardTeam <TryHardTeam@123.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:11:02 by TryHardTeam       #+#    #+#             */
/*   Updated: 2024/12/16 12:11:03 by TryHardTeam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libraries.hpp"
#include "colors.hpp"

// ---------------------  Templates functions  ---------------------

/**
 * @brief Prints an template object with an `endl`
 * 
 * @tparam T 
 * @param obj 
 */
template< typename T >
void	print( const T& obj ){
	
	cout << obj << endl;
}

/**
 * @brief Prints an template object with no `endl`.
 * 
 * @tparam T 
 * @param obj 
 */
template< typename T >
void	printNoEndl( const T& obj ){
	
	cout << obj;
}

/**
 * @brief Prints an template object with the ANSI `COLOR` code with an `endl`. 
 * 
 * @tparam T 
 * @param COLOR 
 * @param obj 
 */
template< typename T >
void	printColor( const string& COLOR, const T& obj ){

	cout << COLOR << obj << RESET << endl;
}

/**
 * @brief Prints an template object with the ANSI `COLOR` code without an `endl`.
 * 
 * @tparam T 
 * @param COLOR 
 * @param obj 
 */
template< typename T >
void	printColorNoEndl( const string& COLOR, const T& obj ){

	cout << COLOR << obj << RESET;
}

//  ------------------ Functions prototypes ------------------ 
void	clearScreen( void );
void	customExit( const string& str, const u_int8_t& exitCode, const string& COLOR = BOLD_RED );
void	extraLine( void );
bool	doesFileExists( const string& fileName);
string	trimWhitespace(const string& str);