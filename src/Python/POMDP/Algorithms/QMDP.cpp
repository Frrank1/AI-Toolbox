#include <AIToolbox/POMDP/Algorithms/QMDP.hpp>

#include <AIToolbox/POMDP/Types.hpp>
#include "../../Utils.hpp"

#include <AIToolbox/MDP/Model.hpp>
#include <AIToolbox/MDP/SparseModel.hpp>
#include <AIToolbox/POMDP/Model.hpp>
#include <AIToolbox/POMDP/SparseModel.hpp>

#include <boost/python.hpp>

using POMDPModelBinded = AIToolbox::POMDP::Model<AIToolbox::MDP::Model>;
using POMDPSparseModelBinded = AIToolbox::POMDP::SparseModel<AIToolbox::MDP::SparseModel>;

template <typename M>
void exportQMDPByModel(std::string className) {
    using namespace AIToolbox::POMDP;
    using namespace boost::python;

    using V = QMDP<M>;

    class_<V>{("QMDP" + className).c_str(), (

         "This class implements the QMDP algorithm for " + className + ".\n"
         "\n"
         "QMDP is a particular way to approach a POMDP problem and solve it\n"
         "approximately. The idea is to compute a solution that disregards the\n"
         "partial observability for all timesteps but the next one. Thus, we\n"
         "assume that after the next action the agent will suddenly be able to\n"
         "see the true state of the environment, and act accordingly. In doing\n"
         "so then, it will use an MDP value function.\n"
         "\n"
         "Remember that only the solution process acts this way. When time to\n"
         "act the QMDP solution is simply applied at every timestep, every\n"
         "time assuming that the partial observability is going to last one\n"
         "step.\n"
         "\n"
         "All in all, this class is pretty much a converter of an\n"
         "MDP::ValueFunction into a POMDP::ValueFunction.\n"
         "\n"
         "Although the solution is approximate and overconfident (since we\n"
         "assume that partial observability is going to go away, we think we\n"
         "are going to get more reward), it is still good to obtain a closer\n"
         "upper bound on the true solution. This can be used, for example, to\n"
         "boost bounds on online methods, decreasing the time they take to\n"
         "converge.\n"
         "\n"
         "The solution returned by QMDP will thus have only horizon 1, since\n"
         "the horizon requested is implicitly encoded in the MDP part of the\n"
         "solution.").c_str(), no_init}

        .def(init<unsigned, double>(
                 "Basic constructor.\n"
                 "\n"
                 "QMDP uses MDP::ValueIteration in order to solve the\n"
                 "underlying MDP of the POMDP. Thus, its parameters (and\n"
                 "bounds) are the same.\n"
                 "\n"
                 "@param horizon The maximum number of iterations to perform.\n"
                 "@param epsilon The epsilon factor to stop the value iteration loop."
        , (arg("self"), "horizon", "epsilon")))

        .def("__call__",                &V::operator(),
                 "This function applies the QMDP algorithm on the input POMDP.\n"
                 "\n"
                 "This function computes the MDP::ValueFunction of the underlying\n"
                 "MDP of the input POMDP with the parameters set. It then converts\n"
                 "this solution into the equivalent POMDP::ValueFunction. Finally\n"
                 "it returns both (plus the boolean specifying whether the epsilon\n"
                 "constraint requested is satisfied or not).\n"
                 "\n"
                 "@param m The POMDP to be solved\n"
                 "\n"
                 "@return A tuple containing a boolean value specifying\n"
                 "whether the specified epsilon bound was reached, a\n"
                 "POMDP::ValueFunction and the equivalent MDP::ValueFunction."
        , (arg("self"), "m"))

        .def("setEpsilon",              &V::setEpsilon,
                 "This function sets the epsilon parameter.\n"
                 "\n"
                 "The epsilon parameter must be >= 0.0, otherwise the function\n"
                 "will throw an std::invalid_argument. The epsilon parameter\n"
                 "sets the convergence criterion. An epsilon of 0.0 forces the\n"
                 "internal ValueIteration to perform a number of iterations\n"
                 "equal to the horizon specified. Otherwise, ValueIteration\n"
                 "will stop as soon as the difference between two iterations\n"
                 "is less than the epsilon specified.\n"
                 "\n"
                 "@param epsilon The new epsilon parameter."
        , (arg("self"), "epsilon"))

        .def("setHorizon",              &V::setHorizon,
                 "This function sets the horizon parameter."
        , (arg("self"), "horizon"))

        .def("getEpsilon",              &V::getEpsilon,
                 "This function returns the currently set epsilon parameter."
        , (arg("self")))

        .def("getHorizon",              &V::getHorizon,
                 "This function returns the currently set horizon parameter."
        , (arg("self")));
}

void exportPOMDPQMDP() {
    using namespace AIToolbox::MDP;

    exportQMDPByModel<POMDPModelBinded>("Model");
    exportQMDPByModel<POMDPSparseModelBinded>("SparseModel");
}
