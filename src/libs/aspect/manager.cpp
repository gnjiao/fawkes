
/***************************************************************************
 *  manager.cpp - Fawkes Aspect Manager
 *
 *  Created: Thu Nov 25 00:34:06 2010 (based on inifin.h)
 *  Copyright  2006-2010  Tim Niemueller [www.niemueller.de]
 *
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version. A runtime exception applies to
 *  this software (see LICENSE.GPL_WRE file mentioned below for details).
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL_WRE file in the doc directory.
 */

#include <aspect/manager.h>
#include <aspect/inifins/blackboard.h>
#include <aspect/inifins/blocked_timing.h>
#include <aspect/inifins/clock.h>
#include <aspect/inifins/configurable.h>
#include <aspect/inifins/fawkes_network.h>
#include <aspect/inifins/logger.h>
#include <aspect/inifins/logging.h>
#include <aspect/inifins/mainloop.h>
#include <aspect/inifins/network.h>
#include <aspect/inifins/plugin_director.h>
#include <aspect/inifins/thread_producer.h>
#include <aspect/inifins/time_source.h>
#include <aspect/inifins/vision_master.h>
#include <aspect/inifins/vision.h>

namespace fawkes {
#if 0 /* just to make Emacs auto-indent happy */
}
#endif

/** @class AspectManager <aspect/manager.h>
 * Aspect and aspect initializer/finalizer manager.
 * This class is the central gatekeeper to aspects for the main application.
 * It manages the initializers/finalizers and thus the aspects which are
 * currently available in the system. It assures that these are not removed
 * before the last thread with an aspect is gone.
 * @author Tim Niemueller
 */

/** Constructor. */
AspectManager::~AspectManager()
{
  std::map<std::string, AspectIniFin *>::iterator i;
  for (i = __default_inifins.begin(); i != __default_inifins.end(); ++i) {
    delete i->second;
  }
  __default_inifins.clear();
}

/** Register initializer/finalizer.
 * @param inifin aspect initializer/finalizer to register
 */
void
AspectManager::register_inifin(AspectIniFin *inifin)
{
  if (__inifins.find(inifin->get_aspect_name()) != __inifins.end()) {
    throw Exception("An initializer for %s has already been registered",
		    inifin->get_aspect_name());
  }
  __inifins[inifin->get_aspect_name()] = inifin;
}

/** Unregister initializer/finalizer.
 * @param inifin aspect initializer/finalizer to unregister
 */
void
AspectManager::unregister_inifin(AspectIniFin *inifin)
{
  if (__inifins.find(inifin->get_aspect_name()) == __inifins.end()) {
    throw Exception("An initializer for %s has not been registered",
		    inifin->get_aspect_name());
  }
  __inifins.erase(inifin->get_aspect_name());
}

void
AspectManager::init(Thread *thread)
{
  Aspect *aspected_thread = dynamic_cast<Aspect *>(thread);
  if (aspected_thread != NULL) { // thread has aspects to initialize
    const std::list<const char *> &aspects = aspected_thread->get_aspects();

    std::list<const char *>::const_iterator i;
    for (i = aspects.begin(); i != aspects.end(); ++i) {
      if (__inifins.find(*i) == __inifins.end()) {
	throw CannotInitializeThreadException("Thread '%s' has the %s, "
					      "but no initilizer is known.",
					      thread->name(), *i);
      }
      __inifins[*i]->init(thread);
    }
  }
}

void AspectManager::finalize(Thread *thread)
{
  Aspect *aspected_thread = dynamic_cast<Aspect *>(thread);
  if (aspected_thread != NULL) { // thread has aspects to finalize
    const std::list<const char *> &aspects = aspected_thread->get_aspects();

    std::list<const char *>::const_iterator i;
    for (i = aspects.begin(); i != aspects.end(); ++i) {
      if (__inifins.find(*i) == __inifins.end()) {
	throw CannotFinalizeThreadException("Thread '%s' has the %s, "
					    "but no finalizer is known.",
					    thread->name(), *i);
      }
      __inifins[*i]->finalize(thread);
    }
  }
}


bool
AspectManager::prepare_finalize(Thread *thread)
{
  Aspect *aspected_thread = dynamic_cast<Aspect *>(thread);
  if (aspected_thread != NULL) { // thread has aspects to finalize
    const std::list<const char *> &aspects = aspected_thread->get_aspects();

    std::list<const char *>::const_iterator i;
    for (i = aspects.begin(); i != aspects.end(); ++i) {
      if (__inifins.find(*i) == __inifins.end()) {
	throw CannotFinalizeThreadException("Thread '%s' has the %s, "
					    "but no finalizer is known.",
					    thread->name(), *i);
      }
      if (!__inifins[*i]->prepare_finalize(thread)) {
	return false;
      }
    }
  }

  return true;
}

/** Register default aspect initializer/finalizer.
 * This loads initializer/finalizer of all aspects which are in the
 * Fawkes aspect library.
 * @param blackboard blackboard for BlackBoardAspect
 * @param collector thread collector for ThreadProducerAspect
 * @param config configuration for ConfigurableAspect
 * @param clock clock for ClockAspect
 * @param logger logger for LoggingAspect
 * @param fnethub Fawkes network hub for FawkesNetworkAspect
 * @param mloop_employer Main loop employer for MainLoopAspect
 * @param logger_employer logger employer for LoggerAspect
 * @param btexec blocked timing executor for MainLoopAspect
 * @param nnresolver network name resolver for NetworkAspect
 * @param service_publisher service publisher for NetworkAspect
 * @param service_browser service browser for NetworkAspect
 * @param pmanager plugin manager for PluginDirectorAspect
 */
void
AspectManager::register_default_inifins(BlackBoard *blackboard,
					ThreadCollector *collector,
					Configuration *config,
					Logger *logger,
					Clock *clock,
					FawkesNetworkHub *fnethub,
					MainLoopEmployer *mloop_employer,
					LoggerEmployer *logger_employer,
					BlockedTimingExecutor *btexec,
					NetworkNameResolver *nnresolver,
					ServicePublisher *service_publisher,
					ServiceBrowser *service_browser,
					PluginManager *pmanager)
{
  if (! __default_inifins.empty())  return;

  BlackBoardAspectIniFin *bb_aif = new BlackBoardAspectIniFin(blackboard);
  BlockedTimingAspectIniFin *bt_aif = new BlockedTimingAspectIniFin();
  ClockAspectIniFin *clock_aif = new ClockAspectIniFin(clock);
  ConfigurableAspectIniFin *conf_aif = new ConfigurableAspectIniFin(config);
  FawkesNetworkAspectIniFin *fnet_aif = new FawkesNetworkAspectIniFin(fnethub);
  LoggerAspectIniFin *logger_aif = new LoggerAspectIniFin(logger_employer);
  LoggingAspectIniFin *log_aif = new LoggingAspectIniFin(logger);
  MainLoopAspectIniFin *mloop_aif = new MainLoopAspectIniFin(mloop_employer,
							     btexec);
  NetworkAspectIniFin *net_aif = new NetworkAspectIniFin(nnresolver,
							 service_publisher,
							 service_browser);
  PluginDirectorAspectIniFin *plug_aif = new PluginDirectorAspectIniFin(pmanager);
  ThreadProducerAspectIniFin *tp_aif = new ThreadProducerAspectIniFin(collector);
  TimeSourceAspectIniFin *ts_aif = new TimeSourceAspectIniFin(clock);
  VisionMasterAspectIniFin *vm_aif = new VisionMasterAspectIniFin();
  VisionAspectIniFin *vis_aif = new VisionAspectIniFin(vm_aif);

  __default_inifins[bb_aif->get_aspect_name()] = bb_aif;
  __default_inifins[bt_aif->get_aspect_name()] = bt_aif;
  __default_inifins[clock_aif->get_aspect_name()] = clock_aif;
  __default_inifins[conf_aif->get_aspect_name()] = conf_aif;
  __default_inifins[fnet_aif->get_aspect_name()] = fnet_aif;
  __default_inifins[logger_aif->get_aspect_name()] = logger_aif;
  __default_inifins[log_aif->get_aspect_name()] = log_aif;
  __default_inifins[mloop_aif->get_aspect_name()] = mloop_aif;
  __default_inifins[net_aif->get_aspect_name()] = net_aif;
  __default_inifins[plug_aif->get_aspect_name()] = plug_aif;
  __default_inifins[tp_aif->get_aspect_name()] = tp_aif;
  __default_inifins[ts_aif->get_aspect_name()] = ts_aif;
  __default_inifins[vm_aif->get_aspect_name()] = vm_aif;
  __default_inifins[vis_aif->get_aspect_name()] = vis_aif;

  std::map<std::string, AspectIniFin *>::iterator i;
  for (i = __default_inifins.begin(); i != __default_inifins.end(); ++i) {
    __inifins[i->first] = i->second;
  }
}

} // end namespace fawkes
