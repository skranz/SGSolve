// This file is part of the SGSolve library for stochastic games
// Copyright (C) 2016 Benjamin A. Brooks
// 
// SGSolve free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// SGSolve is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
// 
// Benjamin A. Brooks
// ben@benjaminbrooks.net
// Chicago, IL

#include "sgsimulationhandler.hpp"

SGSimulationHandler::SGSimulationHandler(QWidget * parent, 
					 const SGSolution & _soln,
					 const SGPoint & _point,
					 int _state)
  : QWidget(parent), soln(_soln), 
    point(_point), state(_state),
    sim(soln)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(tr("SGViewer: Equilibrium simulation"));
  // setWindowModality(Qt::WindowModal);
  
  setMinimumSize(parent->size()/2);

  QMenuBar * menuBar = new QMenuBar();
  menuBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
  QMenu * fileMenu = menuBar->addMenu(tr("&File"));
  QAction * closeAction = new QAction(tr("&Close simulation"),this);
  fileMenu->addAction(closeAction);
  closeAction->setShortcut(tr("Alt+W"));

  QVBoxLayout * mainLayout = new QVBoxLayout();
  mainLayout->addWidget(menuBar);
  
  setLayout(mainLayout);

  QHBoxLayout * ewLayout = new QHBoxLayout();
  
  QFormLayout * controlLayout = new QFormLayout();
  simEdit = new QLineEdit(QString("100"));
  simEdit->setFixedWidth(100);
  controlLayout->addRow(new QLabel(tr("Number of simulations:")),
			simEdit);

  iterationEdit = new QLineEdit(QString("100"));
  iterationEdit->setFixedWidth(100);
  controlLayout->addRow(new QLabel(tr("Number of periods:")),
			iterationEdit);

  QPushButton * simulateButton = new QPushButton(tr("Simulate"));

  simEdit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
  iterationEdit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
  simulateButton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

  controlLayout->addRow(simulateButton);

  QSplitter * editSplitter = new QSplitter();
  editSplitter->setOrientation(Qt::Vertical);
  
  textEdit = new QTextEdit();
  textEdit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
  textEdit->setLineWrapMode(QTextEdit::NoWrap);
  QSizePolicy policy = textEdit->sizePolicy();
  policy.setVerticalStretch(1);
  textEdit->setSizePolicy(policy);
  // controlLayout->addRow(textEdit);
  editSplitter->addWidget(textEdit);

  transitionTableEdit = new QTextEdit();
  transitionTableEdit->setLineWrapMode(QTextEdit::NoWrap);
  transitionTableEdit->setSizePolicy(policy);
  // controlLayout->addRow(transitionTableEdit);
  editSplitter->addWidget(transitionTableEdit);

  editSplitter->setSizePolicy(policy);

  controlLayout->addRow(editSplitter);

  // simulateButton->setFixedWidth(150);

  // controlLayout->addRow(new QSpacerItem(5,5));
  controlLayout->addRow(new QLabel(tr(" ")));


  longRunPayoffEdit = new QLineEdit();
  longRunPayoffEdit->setReadOnly(true);
  longRunPayoffEdit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
  longRunPayoffEdit->setFixedWidth(200);
  controlLayout->addRow(new QLabel(tr("Long run payoffs:")),
			longRunPayoffEdit);

  timeEdit = new QLineEdit();
  timeEdit->setReadOnly(true);
  timeEdit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
  timeEdit->setFixedWidth(200);
  controlLayout->addRow(new QLabel(tr("Elapsed time (s):")),
			timeEdit);
  
  distrPlot = new SGSimulationPlot(soln.getGame().getNumStates());
  distrPlot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

  QScrollArea * scrollArea = new QScrollArea();
  QWidget * scrollWidget = new QWidget();
  QHBoxLayout * scrollLayout = new QHBoxLayout();
  scrollLayout->addWidget(distrPlot);
  scrollWidget->setLayout(scrollLayout);
  scrollArea->setWidget(scrollWidget);

  scrollArea->setWidgetResizable(true);
  scrollArea->setSizePolicy(QSizePolicy::Expanding,
			    QSizePolicy::Preferred);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  QSplitter * ewSplitter = new QSplitter();
  QWidget * controlWidget = new QWidget();
  controlWidget->setLayout(controlLayout);
  ewSplitter->addWidget(controlWidget);
  ewSplitter->addWidget(scrollArea);
  
  mainLayout->addWidget(ewSplitter);

  connect(closeAction,SIGNAL(triggered()),
	  this,SLOT(close()));
  connect(simulateButton,SIGNAL(clicked()),
	  this,SLOT(simulate()));

  // Set up the simulator
  // Find the point that is closest for the given state.
  double minDistance = numeric_limits<double>::max();

  for (list<SGIteration>::const_reverse_iterator iter = soln.getIterations().rbegin();
       iter != soln.getIterations().rend();
       ++iter)
    {
      if (iter->getRevolution() != soln.getIterations().back().getRevolution())
	break;

      double newDistance = ( (iter->getPivot()[state] - point)
			     *(iter->getPivot()[state] - point) );
      if (newDistance < minDistance-1e-7)
	{
	  minDistance = newDistance;
	  
	  initialTuple = iter->getNumExtremeTuples();
	} // if
    } // for
   
  sim.initialize();
  sim.setLogFlag(true);

  // Set up the plots
  stateBars = new QCPBars(distrPlot->xAxis,
			  distrPlot->yAxis);
  distrPlot->addPlottable(stateBars);
  distrPlot->plotLayout()->insertRow(0);
  distrPlot->plotLayout()
    ->addElement(0,0,new QCPPlotTitle(distrPlot,
				      QString("State distribution")));
  
  // Prep x axis with state labels
  QVector<double> ticks;
  QVector<QString> labels;
  
  int numStateTicks = min(soln.getGame().getNumStates(),20);

  for (int state = 0; state < soln.getGame().getNumStates(); state+=soln.getGame().getNumStates()/numStateTicks)
    {
      ticks << state+1;
      labels << QString("S")+QString::number(state);
    } // for state
  distrPlot->xAxis->setAutoTicks(false);
  distrPlot->xAxis->setAutoTickLabels(false);
  distrPlot->xAxis->setTickVector(ticks);
  distrPlot->xAxis->setTickVectorLabels(labels);
  distrPlot->xAxis->setTickLabelRotation(60);
  distrPlot->xAxis->setSubTickCount(0);
  distrPlot->xAxis->setTickLength(0,4);
  distrPlot->xAxis->setRange(0,soln.getGame().getNumStates()+1);

  // Tuple distribution
  distrPlot->plotLayout()
    ->addElement(2,0,new QCPPlotTitle(distrPlot,
				      QString("Tuple distribution")));
  tupleDistrRect = new QCPAxisRect(distrPlot);
  distrPlot->plotLayout()->addElement(3,0,tupleDistrRect);
  distrPlot->addGraph();

  // Prep x axis with tuple labels
  QVector<double> tupleTicks;
  QVector<QString> tupleLabels;
  int numTuplesInLastRev = soln.getIterations().size()-sim.getStartOfLastRev();
  int numTupleTicks = min(numTuplesInLastRev,20);
  for (int tuple = 0; tuple < numTuplesInLastRev; 
       tuple+= numTuplesInLastRev/numTupleTicks)
    {
      tupleTicks << sim.getStartOfLastRev()+tuple;
      tupleLabels << QString("T")+QString::number(sim.getStartOfLastRev()+tuple);
    } // for tuple
  tupleBars = new QCPBars(tupleDistrRect->axis(QCPAxis::atBottom),
			  tupleDistrRect->axis(QCPAxis::atLeft));
  distrPlot->addPlottable(tupleBars);

  tupleDistrRect->axis(QCPAxis::atBottom)->setAutoTicks(false);
  tupleDistrRect->axis(QCPAxis::atBottom)->setAutoTickLabels(false);
  tupleDistrRect->axis(QCPAxis::atBottom)->setTickVector(tupleTicks);
  tupleDistrRect->axis(QCPAxis::atBottom)->setTickVectorLabels(tupleLabels);
  tupleDistrRect->axis(QCPAxis::atBottom)->setTickLabelRotation(60);
  tupleDistrRect->axis(QCPAxis::atBottom)->setSubTickCount(0);
  tupleDistrRect->axis(QCPAxis::atBottom)->setTickLength(0,4);
  tupleDistrRect->axis(QCPAxis::atBottom)->setRange(sim.getStartOfLastRev(),
						    soln.getIterations().back().getIteration());

  // Action distributions
  actionBars = vector<QCPBars *>(soln.getGame().getNumStates());
  actionDistrRects = vector<QCPAxisRect *>(soln.getGame().getNumStates());
  for (int state = 0; state < soln.getGame().getNumStates(); state++)
    {
      distrPlot->plotLayout()->addElement(2*(state+2),0,
					  new QCPPlotTitle(distrPlot,
							   QString("Action distribution in S")
							   +QString::number(state)));

      actionDistrRects[state] = new QCPAxisRect(distrPlot);
      distrPlot->plotLayout()->addElement(2*(state+2)+1,0,actionDistrRects[state]);
      distrPlot->addGraph();

      QVector<double> actionTicks; 
      QVector<QString> actionLabels;
      int numActionTicks = min(20,soln.getGame().getNumActions_total()[state]);
      for (int action = 0; 
	   action < soln.getGame().getNumActions_total()[state];
	   action += soln.getGame().getNumActions_total()[state]/numActionTicks)
	{
	  actionTicks << action+1;
	  actionLabels << QString("A")+QString::number(action);
	}
      actionBars[state] = new QCPBars(actionDistrRects[state]->axis(QCPAxis::atBottom),
				      actionDistrRects[state]->axis(QCPAxis::atLeft));

      distrPlot->addPlottable(actionBars[state]);

      actionDistrRects[state]->axis(QCPAxis::atBottom)->setAutoTicks(false);
      actionDistrRects[state]->axis(QCPAxis::atBottom)->setAutoTickLabels(false);
      actionDistrRects[state]->axis(QCPAxis::atBottom)->setTickVector(actionTicks);
      actionDistrRects[state]->axis(QCPAxis::atBottom)->setTickVectorLabels(actionLabels);
      actionDistrRects[state]->axis(QCPAxis::atBottom)->setTickLabelRotation(60);
      actionDistrRects[state]->axis(QCPAxis::atBottom)->setSubTickCount(0);
      actionDistrRects[state]->axis(QCPAxis::atBottom)->setTickLength(0,4);
      actionDistrRects[state]->axis(QCPAxis::atBottom)->setRange(0,soln.getGame().getNumActions_total()[state]+1);
    } // for action
} // constructor

void SGSimulationHandler::simulate()
{
  QTime time;

  time.start();

  sim.simulate(simEdit->text().toInt(),
	       iterationEdit->text().toInt(),
	       state,initialTuple);
  
  SGPoint payoffs = sim.getLongRunPayoffs();
  longRunPayoffEdit->setText(QString("(")
			     +QString::number(payoffs[0])
			     +QString(",")
			     +QString::number(payoffs[1])
			     +QString(")"));

  // Prep x axis with state labels
  QVector<double> stateDistr;
  QVector<double> ticks;
  double stateMax = 0.0;
  for (int state = 0; state < soln.getGame().getNumStates(); state++)
    {
      ticks << state+1;

      double tempProb = (1.0*sim.getStateDistr()[state])/(1.0*sim.getNumIter());
      stateDistr << tempProb;
      stateMax = max(stateMax,tempProb);
    } // for state
  distrPlot->yAxis->setRange(-0.05,stateMax+0.05);
  stateBars->setData(ticks,stateDistr);

  // Tuple distribution
  QVector<double> tupleDistr;
  QVector<double> tupleX;
  double tupleMax = 0.0;
  for (int tuple = 0; tuple < sim.getTupleDistr().size(); tuple++)
    {
      double tempProb = (1.0*sim.getTupleDistr()[tuple])/(1.0*sim.getNumIter());
      tupleDistr << tempProb;
      tupleX << sim.getStartOfLastRev()+tuple;
      tupleMax = max(tupleMax,tempProb);
    } // for tuple
  tupleDistrRect->axis(QCPAxis::atLeft)->setRange(0,tupleMax+0.05);

  tupleBars->setData(tupleX,tupleDistr);

  // Action distributions
  for (int state = 0; state < soln.getGame().getNumStates(); state++)
    {
      QVector<double> actionDistr;
      QVector<double> actionTicks; 
      double actionMax = 0.0;

      for (int action = 0; action < soln.getGame().getNumActions_total()[state];
	   action++)
	{
	  actionTicks << action+1;
	  double tempProb = (1.0*sim.getActionDistr()[state][action])/
	    (1.0*sim.getStateDistr()[state]);
	  actionDistr << tempProb;
	  actionMax = max(actionMax,tempProb);
	}
      actionDistrRects[state]->axis(QCPAxis::atLeft)->setRange(-0.05,actionMax+0.05);

      actionBars[state]->setData(actionTicks,actionDistr);
    } // for action

  textEdit->setText(QString::fromStdString(sim.getStringStream().str()));
  transitionTableEdit->setText(QString::fromStdString(sim.getTransitionTableStringStream().str()));
  timeEdit->setText(QString::number(time.elapsed()/1000.0));

  distrPlot->replot();
  
} // simulate
