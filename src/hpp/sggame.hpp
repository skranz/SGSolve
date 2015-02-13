#ifndef _SGGAME_HPP
#define _SGGAME_HPP

#include "sgcommon.hpp"
#include "sgutilities.hpp"
#include "sgexception.hpp"
#include "sgtuple.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/utility.hpp>

//! Describes a stochastic game
/*! This class contains members that describe a stochastic game. */
class SGGame
{
private:
  double delta; /*!< The discount factor. */
  int numPlayers; /*!< The number of players, always 2. */
  int numStates; /*!< The number of states, must be at least 1. */
  vector< vector<int> > numActions; /*!< Gives the number of each
                                       player's actions in each
                                       state. In particular, player i
                                       has numActions[s][i] actions in
                                       state s. Should note that a
                                       pair (a1,a2) is mapped into an
                                       action profile using the
                                       formula
                                       a=a1+a2*numActions[s][a1]. */
  vector<int> numActions_total; /*!< Total number of action profiles
                                   for each state. */

  vector< vector<SGPoint> > payoffs; /*!< Gives the payoffs of the
                                        players as a function of the
                                        action profile. In particular,
                                        payoffs[s][a][i] are player
                                        i's payoffs in state s when
                                        action profile a is played. */

  vector< vector< vector<double> > > probabilities; /*!< State
                                                       transition
                                                       probabilities:
                                                       probabilities[s][a][s']
                                                       is the
                                                       probability of
                                                       transitioning
                                                       to state s'
                                                       when action
                                                       profile a is
                                                       played in state
                                                       s. */
  vector< list<int> > eqActions; /*!< Lists of action profiles that
				   are allowed to be played in each
				   state. By default, initialized to
				   all action profiles. Allows one to,
				   for example, look at strongly
				   symmetric equilibria (by first
				   excluding asymmetric action
				   profiles from the lists). Players
				   can always deviate to action
				   profiles outside of this list. */

  vector<bool> unconstrained; /*!< If unconstrained[i]=true, the
                                 algorithm will not impose incentive
                                 compatibility as a constraint for
                                 player i. */
  
  //! Serializes the game using boost.
  template <class Archive> 
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & delta & numPlayers & numStates
      & numActions & numActions_total;
    ar & payoffs & probabilities & eqActions & unconstrained;
  }

public:
  //! Default constructor.
  SGGame():
    numStates(1),
    numActions(1,vector<int>(2,1)),
    numActions_total(1,1),
    payoffs(1,vector<SGPoint>(1,SGPoint(0,0))),
    probabilities(1,vector< vector<double> > (1,vector<double> (1,1))),
    delta(0.9),
    numPlayers(2),
    unconstrained(2,false)
  {
  }
    
  ~SGGame() {}
  //! Constructor excluding eqActions
  /*!< Initializes a game object with all actions being
     permissible. */
  SGGame(double _delta,
	 int _numStates,
	 const vector< vector<int> > & _numActions,
	 const vector< vector< vector<double> > > & _payoffs,
	 const vector< vector< vector<double> > > & _probabilities);

  SGGame(double _delta,
	 int _numStates,
	 const vector< vector<int> > & _numActions,
	 const vector< vector< vector<double> > > & _payoffs,
	 const vector< vector< vector<double> > > & _probabilities,
	 const vector<bool> & _unconstrained);

  //! Constructor with custom equilibrium actions
  /*!< If _eqActions is an empty vector, the constructor will
     initialize it so that all action profiles are allowed. Otherwise,
     we restrict attention to equilibria in which action profiles in
     _eqActions are used. */
  SGGame(double _delta,
	 int _numStates,
	 const vector< vector<int> > & _numActions,
	 const vector< vector< vector<double> > > & _payoffs,
	 const vector< vector< vector<double> > > & _probabilities,
	 const vector< list<int> > & _eqActions,
	 const vector<bool> & _unconstrained);

  //! Returns SGGame::delta, the discount factor
  double getDelta() const { return delta; }  
  //! Returns SGGame::numPlayers, the number of players (always 2)
  int getNumPlayers() const { return numPlayers; }
  //! Returns the number of states.
  int getNumStates() const { return numStates; }
  //! Sets the argument _numActions equal to SGGame::numActions
  const vector< vector<int> > & getNumActions() const
  { return numActions; }
  //! Returns a constant reference to the total number of actions in
  //! each state
  const vector<int> & getNumActions_total () const
  { return numActions_total; }
  //! Returns a const reference to probabilities
  const vector< vector< vector<double> > > & getProbabilities() const
  {return probabilities;}
  //! Returns a const reference to the payoffs
  const vector< vector<SGPoint> > & getPayoffs() const
  { return payoffs; }
  //! Returns a const reference to the equilibrium actions
  const vector< list<int> > & getEquilibriumActions() const
  { return eqActions; }
  //! Sets the arguments equal to tight upper and lower bounds on the
  //! payoffs, respectively.
  void getPayoffBounds(SGPoint & UB, SGPoint & LB) const;

  const vector<bool> & getConstrained() const
  { return unconstrained; }

  bool setDiscountFactor(double newDelta);
  bool setPayoff(int state, int action, int player, double payoff);
  bool setProbability(int state, int action,
		      int newState, double prob);
  bool setConstrained(const vector<bool> & _unconstrained);
  bool addAction(int state, int player, int position);
  bool removeAction(int state, int player, int action);
  bool addState(int position);
  bool removeState(int state);
  
  //! Static method for saving an SGGame object to the file filename.
  static void save(const SGGame & game, const char* filename)
  {
    std::ofstream ofs(filename);

    if (ofs.good())
      {
	boost::archive::text_oarchive oa(ofs);
	oa << game;
      }
    else
      throw(SGException(SGException::FAILED_OPEN));
  }

  //! Static method for loading an SGGame object from the file filename.
  static void load(SGGame & game, const char* filename)
  {
    std::ifstream ifs(filename);
    if (ifs.good())
      {
	boost::archive::text_iarchive ia(ifs);
	ia >> game;
      }
    else
      throw(SGException(SGException::FAILED_OPEN));
  }

  friend class boost::serialization::access;
  friend class SGSolver;
  friend class SGApprox;
};

#endif