#include "sgenv.hpp"

SGEnv::SGEnv()
{
  restoreDefaults();
}

void SGEnv::restoreDefaults()
{
  maxIterations = 1e6;
  maxUpdatePivotPasses = 1e6;
  tupleReserveSize = 1e4;

  errorTol = 1e-8;
  directionTol = 1e-13;
  pastThreatTol = 1e-10;
  updatePivotTol = 1e-13;
  ICTol = 1e-12;
  normTol = 1e-12;
  flatTol = 1e-10;
  levelTol = 1e-12;
  improveTol = 1e-13;
  backBendingTol = 1e-6;
  movementTol = 1e-14; 
  roundTol = 0.0;
  intersectTol = 1e-10;

  mergeTuples = false;
  backBendingWarning = false;
  storeIterations = true;
  printToCout = true;

  setOStream(cout);
}

void SGEnv::setParam(SGEnv::DBL_PARAM param, double value)
{
  if (value<0)
    throw(SGException(SGException::BAD_PARAM_VALUE));

  switch (param)
    {
    case ERRORTOL:
      errorTol = value;
      break;

    case DIRECTIONTOL:
      directionTol = value;
      break;

    case PASTTHREATTOL:
      pastThreatTol = value;
      break;
      
    case UPDATEPIVOTTOL:
      updatePivotTol = value;
      break;

    case ICTOL:
      ICTol = value;
      break;

    case NORMTOL:
      normTol = value;
      break;

    case FLATTOL:
      flatTol = value;
      break;

    case LEVELTOL:
      levelTol = value;
      break;

    case IMPROVETOL:
      improveTol = value;
      break;

    case ROUNDTOL:
      roundTol = value;
      break;
      
    case BACKBENDINGTOL:
      backBendingTol = value;
      break;

    case MOVEMENTTOL:
      movementTol = value;
      break;

    default:
      throw(SGException(SGException::UNKNOWN_PARAM));
    }

} // setParam

void SGEnv::setParam(SGEnv::BOOL_PARAM param, bool value)
{
  switch (param)
    {
    case BACKBENDINGWARNING:
      backBendingWarning = value;
      break;

    case MERGETUPLES:
      mergeTuples = value;
      break;

    case STOREITERATIONS:
      storeIterations = value;
      break;

    case PRINTTOLOG:
      printToLog = value;
      break;
      
    case PRINTTOCOUT:
      printToCout = value;
      break;
      
    default:
      throw(SGException(SGException::UNKNOWN_PARAM));
    }
} // setParam

void SGEnv::setParam(SGEnv::INT_PARAM param, int value)
{
  if (value<0)
    throw(SGException(SGException::BAD_PARAM_VALUE));

  switch (param)
    {
    case MAXITERATIONS:
      maxIterations = value;
      break;
      
    case MAXUPDATEPIVOTPASSES:
      maxUpdatePivotPasses = value;
      break;

    case TUPLERESERVESIZE:
      tupleReserveSize = value;
      break;

    default:
      throw(SGException(SGException::UNKNOWN_PARAM));
    }
} // setParam