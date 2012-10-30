//=======================================================================
// Copyright Baptiste Wicht 2011-2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef MTAC_BASIC_BLOCK_H
#define MTAC_BASIC_BLOCK_H

#include <vector>

#include "variant.hpp"

#include "mtac/forward.hpp"

#include "ltac/forward.hpp"

namespace eddic {

class FunctionContext;
class Context;

namespace mtac {

/*!
 * \class BasicBlock
 * \brief A basic block in the MTAC representation. 
 * The basic blocks of a function are maintained in a doubly linked list. 
 */
class BasicBlock {
    public:
        typedef std::vector<mtac::Statement>::iterator iterator;

        /*!
         * Create a new basic block with the given index. 
         * \param The index of the basic block
         */
        BasicBlock(int index);
        
        /*!
         * Return an iterator to the first statement. 
         * \return an iterator to the first statement.
         */
        iterator begin();
        
        /*!
         * Return an iterator one past the last statement. 
         * \return an iterator one past the last statement.
         */
        iterator end();

        /*!
         * Add a new statement to the basic block. 
         * \param statement The statement to add. 
         */
        void add(mtac::Statement statement);

        const int index;    /*!< The index of the block */
        unsigned int depth = 0;
        std::string label;  /*!< The label of the block */
        std::shared_ptr<FunctionContext> context = nullptr;     /*!< The context of the enclosing function. */

        std::vector<mtac::Statement> statements;    /*!< The MTAC statements inside the basic block. */
        
        std::vector<ltac::Statement> l_statements;  /*!< The LTAC statements inside the basic block. */

        /* Doubly-linked list  */

        std::shared_ptr<BasicBlock> next = nullptr;     /*!< The next basic block in the doubly-linked list. */
        std::shared_ptr<BasicBlock> prev = nullptr;     /*!< The previous basic block in the doubly-linked list. */

        /* Control Flow Graph */
        std::vector<std::shared_ptr<BasicBlock>> successors;
        std::vector<std::shared_ptr<BasicBlock>> predecessors;

        /* Dominance tree */
        
        std::shared_ptr<BasicBlock> dominator = nullptr;     /*!< The immediate dominator of this basic block. */
};

typedef std::shared_ptr<BasicBlock> basic_block_p;

std::ostream& operator<<(std::ostream& stream, BasicBlock& basic_block);
std::ostream& operator<<(std::ostream& stream, basic_block_p& basic_block);

mtac::BasicBlock::iterator begin(mtac::basic_block_p function);
mtac::BasicBlock::iterator end(mtac::basic_block_p function);

} //end of mtac

} //end of eddic

#endif
