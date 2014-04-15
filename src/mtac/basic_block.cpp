//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "assert.hpp"

#include "mtac/Function.hpp"

using namespace eddic;

mtac::basic_block::basic_block(int i) : index(i), label("") {}
        
mtac::Quadruple& mtac::basic_block::find(std::size_t uid){
    for(auto& quadruple : statements){
        if(quadruple.uid() == uid){
            return quadruple;
        }
    }

    eddic_unreachable("The uid should exists");
}

std::size_t mtac::basic_block::size() const {
    return statements.size();
}

std::size_t mtac::basic_block::size_no_nop() const {
    std::size_t size = 0;

    for(auto& quadruple : statements){
        if(quadruple.op != mtac::Operator::NOP){
            ++size;
        }
    }

    return size;
}

std::ostream& mtac::operator<<(std::ostream& stream, const std::shared_ptr<basic_block>& basic_block){
    if(basic_block){
        return stream << *basic_block;
    } else {
        return stream << "null_bb";
    }
}

std::ostream& mtac::operator<<(std::ostream& stream, const basic_block& block){
    if(block.index == -1){
        return stream << "ENTRY";
    } else if(block.index == -2){
        return stream << "EXIT";
    } else {
        return stream << "B" << block.index;
    }
}

mtac::basic_block::iterator mtac::begin(mtac::basic_block_p block){
    return block->begin();
}

mtac::basic_block::iterator mtac::end(mtac::basic_block_p block){
    return block->end(); 
}
    
void pretty_print(std::vector<mtac::basic_block_p> blocks, std::ostream& stream){
    if(blocks.empty()){
        stream << "{}";
    } else {
        stream << "{" << blocks[0];

        for(std::size_t i = 1; i < blocks.size(); ++i){
            stream << ", " << blocks[i];
        }

        stream << "}";
    }
}

mtac::basic_block_p mtac::clone(mtac::Function& function, mtac::basic_block_p block){
    auto new_bb = function.new_bb();

    //Copy the control flow graph properties, they will be corrected after
    new_bb->successors = block->successors;
    new_bb->predecessors = block->predecessors;

    //Copy all the statements
    new_bb->statements = block->statements;
    
    return new_bb;
}

void mtac::pretty_print(std::shared_ptr<const mtac::basic_block> block, std::ostream& stream){
    std::string sep(25, '-');

    stream << sep << std::endl;
    stream << *block;

    stream << " prev: " << block->prev << ", next: " << block->next << ", dom: " << block->dominator << std::endl;
    stream << "successors "; ::pretty_print(block->successors, stream); stream << std::endl;;
    stream << "predecessors "; ::pretty_print(block->predecessors, stream); stream << std::endl;;

    stream << sep << std::endl;
}
