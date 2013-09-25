// Copyright (C) 2010 - 2012 Dirk Eddelbuettel and Romain Francois
// Copyright (C) 2013 Romain Francois
//
// This file is part of Rcpp11.
//
// Rcpp11 is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp11 is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp11.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Rcpp_grow_h
#define Rcpp_grow_h

#include <RcppCommon.h>
#include <Rcpp/Named.h>

namespace Rcpp {

    // inline SEXP pairlist() { return R_NilValue ; }
    SEXP grow( SEXP head, SEXP tail ) ; 
    
    namespace internal{
     
    template <typename T>
    	inline SEXP grow__dispatch( std::false_type, const T& head, SEXP tail ){
    	    return grow( wrap(head), tail ) ;
    	}
    
    	template <typename T>
    	inline SEXP grow__dispatch( std::true_type, const T& head, SEXP tail ){
    	    SEXP y = PROTECT( wrap( head.object) ) ;
    	    SEXP x = PROTECT( Rf_cons( y , tail) ) ;
    	    SEXP headNameSym = ::Rf_install( head.name.c_str() );
    	    SET_TAG( x, headNameSym ); 
    	    UNPROTECT(2); 
    	    return x; 	
    	}
    
    } // internal

    /**
     * grows a pairlist. First wrap the head into a SEXP, then 
     * grow the tail pairlist
     */
    template <typename T>
    SEXP grow(const T& head, SEXP tail) {
        return internal::grow__dispatch( typename traits::is_named<T>::type(), head, tail );
    }
    
    template <typename... Args>
    struct PairlistHelper ;
    
    template <typename First, typename... Args>
    struct PairlistHelper<First, Args...>{
        static inline SEXP get(const First& first, const Args&... pack){
            return grow( first, PairlistHelper<Args...>::get( pack... ) ) ; 
        }
    } ;
    
    template <>
    struct PairlistHelper<>{
        static inline SEXP get(){ return R_NilValue; }
    } ;
    
    
    template <typename... Args>
    SEXP pairlist( const Args&... args ){
        return PairlistHelper<Args...>::get( args... ) ;    
    }
     
    // /* end of the recursion, wrap first to make the CAR and use R_NilValue as the CDR of the list */
    // 
    // template<typename T, typename... Args>
    // SEXP pairlist( const T& first, const Args&... args ){
    //     return grow(first, pairlist(args...) );
    // }

} // namespace Rcpp

#endif
