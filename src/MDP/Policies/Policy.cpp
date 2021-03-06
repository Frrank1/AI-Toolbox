#include <AIToolbox/MDP/Policies/Policy.hpp>

#include <algorithm>

#include <AIToolbox/Utils/Core.hpp>
#include <AIToolbox/Utils/Probability.hpp>

namespace AIToolbox {
    namespace MDP {
        Policy::Policy(const size_t s, const size_t a) :
                PolicyInterface(s, a), policy_(S, A)
        {
            // Random policy is default
            policy_.fill(1.0/getA());
        }

        Policy::Policy(const PolicyInterface & p) :
                PolicyInterface(p.getS(), p.getA()), policy_(S, A)
        {
            for ( size_t s = 0; s < S; ++s )
                for ( size_t a = 0; a < A; ++a )
                    policy_(s, a) = p.getActionProbability(s, a);
        }

        Policy::Policy(const size_t s, const size_t a, const ValueFunction & v) :
                PolicyInterface(s, a), policy_(S, A)
        {
            const auto & actions = std::get<ACTIONS>(v);
            for ( size_t s = 0; s < S; ++s )
                policy_(s, actions[s]) = 1.0;
        }

        size_t Policy::sampleAction(const size_t & s) const {
            return sampleProbability(A, policy_.row(s), rand_);
        }

        double Policy::getActionProbability(const size_t & s, const size_t & a) const {
            return policy_(s, a);
        }

        Vector Policy::getStatePolicy(const size_t s) const {
            return policy_.row(s);
        }

        void Policy::setStatePolicy(const size_t s, const Vector & p) {
            auto sum = p.sum();
            if ( checkDifferentSmall(sum, 1.0) ) throw std::invalid_argument("Setting state policy not summing to 1");

            policy_.col(s) /= sum;
        }

        void Policy::setStatePolicy(const size_t s, const size_t a) {
            policy_.row(s).fill(0.0);
            policy_(s, a) = 1.0;
        }

        const Policy::PolicyTable & Policy::getPolicyTable() const {
            return policy_;
        }
    }
}
