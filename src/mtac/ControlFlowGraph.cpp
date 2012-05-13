//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include "mtac/ControlFlowGraph.hpp"

using namespace eddic;
        
mtac::ControlFlowGraph::ControlFlowGraph(){
    graph = std::make_shared<InternalControlFlowGraph>();
}

std::pair<mtac::ControlFlowGraph::BasicBlockIterator, mtac::ControlFlowGraph::BasicBlockIterator> mtac::ControlFlowGraph::blocks(){
    return boost::vertices(*graph);
}

std::pair<mtac::ControlFlowGraph::EdgeIterator, mtac::ControlFlowGraph::EdgeIterator> mtac::ControlFlowGraph::edges(){
    return boost::edges(*graph);
}
