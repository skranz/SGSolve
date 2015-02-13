// Classes for storing data for stochastic games.
#ifndef _SGACTION_HPP
#define _SGACTION_HPP

#include "sgcommon.hpp"
#include "sgtuple.hpp"
#include "sgenv.hpp"
#include "sggame.hpp"

//! Describes an action in the game
/*! Stores IC region information for a single action. Stores the
action, minimum incentive compatible payoffs, points of intersection
between the IC region and the expected feasible set, and indices of
the tuples that generate the points of intersection. */
class SGAction
{
protected:
  const SGEnv & env; /*!< Constant reference to the parent
                        environment. */

  int state; /*!< The state in which this action profile can be played. */
  int action; /*!< The index of the action profile. */
  SGPoint minIC; /*!< The minimum continuation value to support
                    incentive compatibility, relative to the current
                    threat tuple. In particular, this is the maximum
                    over all deviations of the expected threat point
                    under the deviation, plus (1-delta)/delta times
                    the static gains from deviating. */
  vector< SGTuple > points; /*!< Extreme points of the set of expected
                               feasible continuation values at which
                               some player's incentive constraint
                               binds. points[i] is an SGTuple
                               consisting of either 0 or 2 SGPoint
                               objects, which are the extreme binding
                               payoffs on player i's incentive
                               constraint. The convention is that the
                               first element of the tuple is the
                               northern or easternmost of the two
                               binding payoffs. */ 

  // vector< vector<SGPoint> > slopes; /*!< These are the slopes of the
  //                                      boundary expected feasibile set
  //                                      that extends clockwise from the
  //                                      binding point. */

  vector< vector< int > > tuples; /*!< The vector tuples[i][j] points
                                     to the element of
                                     SGApproximation::extremeTuples
                                     whose expectation is just
                                     clockwise relative to
                                     points[i][j]. */
  bool isNull; /*!< Flag to indicate that this is the place holder
                  "null" action. */

  bool corner; /*!< Flag that indicates that the feasible set has a
                  corner. */


public:
  // //! Default constructor
  // SGAction() {}
  //! Initializes the action with the given state/action
  SGAction(const SGEnv & _env):
    env(_env),
    isNull(true)
  {}

  SGAction(const SGEnv & _env, int _state, int _action);
  ~SGAction() {}

  bool getIsNull() const {return isNull;}

  //! Returns the action
  int getAction() const { return action; }
  //! Returns the state
  int getState() const { return state; }
  //! Returns the minimum IC continuation values
  SGPoint getMinICPayoffs() const {return minIC;}
  //! Trims binding continuation segments
  /*! Intersects the binding continuation segments in SGAction::points
      with the half space that is below pivot in
      direction.getNormal(). */
  void intersectRay(const SGPoint & pivot, 
  		    const SGPoint & direction);
  //! Static method to carry out trimming operations
  /*! */
  void intersectRaySegment(const SGPoint & pivot,
			   const SGPoint & direction,
			   int player);

  //! Calculates the minimum incentive compatible continuation payoff
  void calculateMinIC(const SGGame & game,
		      const vector<bool> & update,
		      const SGTuple & threatTuple);

  static double calculateMinIC(int action,int state, int player,
			       const SGGame & game,
			       const SGTuple & threatTuple);

  //! Serializes the action using the boost::serialization library
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & state & action & minIC 
      & points & tuples;
  }

  friend class boost::serialization::access;
  friend class SGApprox;
}; // SGAction


#endif