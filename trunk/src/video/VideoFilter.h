// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Video_Filter_h
#define Video_Filter_h

#include "Parameters.h"
#include "AsynchSuccLink.h"
#include "AsynchLinkManager.h"
#include "VideoFilterLink.h"
#include "VideoBrokerLink.h"
#include "BufferManager.h"

#include "miro/TimeSeries.h"
#include "miro/Log.h"
#include "idl/VideoC.h"

#include <vector>
#include <string>

#define FILTER_PARAMETERS_FACTORY(X) \
    virtual X ## Parameters * getParametersInstance() const; \
    virtual X ## Parameters * getParametersInstance(const ::Video::FilterParameters& _p) const

#define FILTER_PARAMETERS_FACTORY_IMPL(X) \
  X ## Parameters * \
  X::getParametersInstance() const \
  { \
    return new X ## Parameters(); \
  } \
  X ## Parameters * \
  X::getParametersInstance(const ::Video::FilterParameters& _p) const \
  { \
    X ## Parameters const& p= dynamic_cast<X ## Parameters const&>(_p); \
    return new X ## Parameters(p); \
  }

#define IMAGE_PARAMETERS_FACTORY(X) \
    virtual Video::FilterImageParameters * getImageParametersInstance() const; \
    virtual Video::FilterImageParameters * getImageParametersInstance(const ::Video::FilterImageParameters& _p) const

#define IMAGE_PARAMETERS_FACTORY_IMPL(X) \
  Video::FilterImageParameters * \
  X::getImageParametersInstance() const \
  { \
    return new X ## ImageParameters(); \
  } \
  Video::FilterImageParameters * \
  X::getImageParametersInstance(const ::Video::FilterImageParameters& _p) const \
  { \
    X ## ImageParameters const& p= dynamic_cast<X ## ImageParameters const&>(_p); \
    return new X ## ImageParameters(p); \
  }

/* In contrast to the IMAGE_PARAMETERS_FACTORY_IMPL version above, this one uses the
   name Y to assign the method to the right class. */
#define IMAGE_PARAMETERS_FACTORY_IMPL_2(X, Y) \
  Video::FilterImageParameters * \
  Y::getImageParametersInstance() const \
  { \
    return new X ## ImageParameters(); \
  } \
  Video::FilterImageParameters * \
  Y::getImageParametersInstance(const ::Video::FilterImageParameters& _p) const \
  { \
    X ## ImageParameters const& p= dynamic_cast<X ## ImageParameters const&>(_p); \
    return new X ## ImageParameters(p); \
  }


// forward declarations
namespace Miro
{
  class Server;
  class VideoImpl;
  class FilterTreeIDL;
  class ConfigDocument;
}

namespace Video
{
  // forward declarations
  class FilterParameters;
  class VideoInterfaceParameters;
  class BufferManager;
  class Device;

  //! Base class of all video filters.
  /**
   * Filter objects are instanciated during the construction of the
   * filter tree by a Video::FilterFactory<> instance, registered at
   * the Video::FilterRepository. This process is triggered by
   * @ref Video::Service::buildFilterTree().
   *
   * The requested input format for the filter is provided as
   * parameter to the constructor. The order of construction
   * is a prefix depth first traversion of the filter tree as specified in
   * the configuration file.
   *
   * After construction of the tree, the filters init() methods are
   * called in the same order. That is, predecessor filters are
   * fully initialized, successor filters not.
   *
   * After initialization, the Video::Consumer thread evaluates the
   * filter tree in depth first order. 
   *
   * On termination, the fini() method of each filter is called in
   * reversed prefix depth first order.
   * Afterwards the filters are destructed in the same order. 
   */
  class Filter
  {
  public:
    //! Initializing constructor.
    Filter(Miro::ImageFormatIDL const& _inputFormat);
    //! Virtual destructor for cleanup.
    virtual ~Filter();

    // Factory method specification for parameters.
    FILTER_PARAMETERS_FACTORY(Filter);
    IMAGE_PARAMETERS_FACTORY(Filter);

    //! Flag indicating whether setting up an instance of the Video interface for the filter is allowed.
    bool interfaceAllowed() const throw ();
    Miro::VideoImpl * interface() const throw();
    //! Accessor to the filters buffer manager.
    BufferManager * bufferManager() throw();
    //! Const accessor to the filters buffer manager.
    BufferManager const * bufferManager() const throw();

    //! The input format the filter instance is working on.
    const Miro::ImageFormatIDL& inputFormat() const;
    //! The output format the filter instance is working on.
    const Miro::ImageFormatIDL& outputFormat() const;

    //! Flag indicating whether the filter is working in place.
    /**
     * This is not implemented yet. Currently all filters work
     * on different input and output buffers.
     */
    bool inplace() const;

    //! Initialization of the filter tree.
    void initTree(Miro::Server& _server, Miro::ConfigDocument& _config);
    //! Finalization of the filter tree.
    void finiTree();
    //! Set filter name.
    /** 
     * Only to be used by Video::Service::buildFilterTree() on filter 
     * construction.
     */
    void name(std::string const& _name);
    //! Get filter name.
    /**
     * The name of each filter instance is specified in the configuration
     * file.
     */
    std::string const& name() const;


    //! Process the filter tree recoursivly.
    virtual void processFilterTree();
    //! Collect the performance statistics of the filter tree recoursivly.
    virtual void filterTreeStats(Miro::FilterTreeIDL& _tree) const;

    //! Set the parent filter.
    void setPredecessor(Filter * _pre);
    //! Add a predecessor link filter.
    void addPredecessorLink(Filter * _pre);
    
    // connection management

    //! Flag indicating that the filter is active.
    bool active() const;
    //! Number of VideoService connections to the filter.
    unsigned int connections() const;

    // callbacks for connection management by VideoImpl

    //! A client connects to the filter.
    void connect();
    //! A client disconnects from the filter.
    void disconnect();

    //! Search the filter tree for a filter by its name.
    Filter * findByName(std::string const & _name);
    //! Search the filter tree for a filter by its interface name.
    Filter * findByInterfaceName(std::string const & _name);
    //! Number of image buffers provided
    /**
     * This is actually a configuration parameter and mostly 
     * needed by the Video interface.
     */
    unsigned int outputBuffers() const throw();

    //! Accessor method for the filters paramters.
    FilterParameters const * parameters() const throw();

    //! Deferred initialization: set synch mode.
    void setSynchMode(bool _synchWithCurrent, bool _synchAllNew) throw();

    //! Return parent node of the filter tree.
    Filter * parentNode() throw();
    //! Return the root node of the filter tree.
    Filter * rootNode() throw();
    //! Return the root node of the filter tree.
    Device * rootDevice() throw();

  protected:
    //--------------------------------------------------------------------------
    // protected methods

    //! Factory method for BufferManager instances.
    /**
     * DeviceFilters normally use their own buffer manager.
     */
    virtual BufferManager * bufferManagerInstance() const;
    //! Initialization of the filter.
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    //! Do the work.
    virtual void process();
    //! Finalization of the filter.
    virtual void fini();

    // Buffer management

    //! Get address of input buffer.
    unsigned char const * inputBuffer() const;
    //! Get index of input buffer.
    unsigned long inputBufferIndex() const;
    //! Get address of input buffer parameters.
    FilterImageParameters const * inputBufferParameters() const;
    //! Set address of input buffer and its parameters
    void inputBuffer(unsigned long _index, 
		     unsigned char const * _buffer,
		     FilterImageParameters const * _parms);
    //! Get address of output buffer.
    unsigned char * outputBuffer();
    //! Get address of output parameters parameters.
    FilterImageParameters * outputBufferParameters();
    //! Get address of linked input buffer.
    unsigned char const * inputBufferLink(unsigned long _index) const;
    //! Get address of linked input parameters.
    FilterImageParameters const * inputBufferParametersLink(unsigned long _index) const;
    //! Set address of linked input buffer and its parameters.
    void inputBufferLink(unsigned long _linkIndex,
			 unsigned long _bufferIndex, 
			 unsigned char const * _buffer,
			 FilterImageParameters const * _params);
    //! Set the image index of the pending video broker getNextImageSet requests.
    virtual void setBrokerRequests();

  public:
    //! Add a broker request for the next image.
    /** This is done synchronised by the Video::Device */
    void addBrokerRequest(BrokerLink * _brokerRequest);

    //! Get filter number in tree.
    unsigned int number() const throw();
    
  protected:
    //--------------------------------------------------------------------------
    // protected methods
    //--------------------------------------------------------------------------

    // connection management

    //! Internal connect method (no locking).
    void protectedConnect();
    //! Internal disconnect method (no locking).
    void protectedDisconnect();
    //! Internal calcConnectivity method (no locking).
    void protectedCalcConnectivity();

    //! Backward linking method for double linking.
    /** Called from _succ->setPredecessor() */
    void addSuccessor(Filter * _succ);
    //! Backward linking method for double linking.
    /** Called from _succ->addPredecessorLink() */
    void addSuccessorLink(Filter * _succ, unsigned long _index);
    //! Backward linking method for double linking.
    /** Called from _succ->addAsynchPredecessorLink() */
    void addAsynchSuccessorLink(AsynchSuccLink const& _succLink);

    //! Try to acquire the asynchronous buffers. 
    bool tryAcquireAsynchBuffers() { return true; }

    //! Search the filter subtree for a filter by its name.
    Filter * findByNameDown(std::string const & _name);

    //! Shortcut for inputFormat_.width
    unsigned int width() const throw();
    //! Shortcut for inputFormat_.height
    unsigned int height() const throw();

    //--------------------------------------------------------------------------
    // protected types
    //--------------------------------------------------------------------------

    //! A std::vector of Video::Filter pointers.
    typedef std::vector<Filter *> FilterVector;

    //! A std::vector of Video::FilterSuccLink objects.
    typedef std::vector<FilterSuccLink> FilterSuccVector;

    //! A std::vector of Video::FilterSuccLink objects.
    typedef std::vector<FilterPreLink> FilterPreVector;

    //! A std::vector of Video::FilterSuccLink objects.
    typedef std::vector<AsynchSuccLink> AsynchSuccVector;

    //--------------------------------------------------------------------------
    // protected object data
    //--------------------------------------------------------------------------

    //! Specification of the input format the filter instance is processing.
    Miro::ImageFormatIDL inputFormat_;
    //! Specification of the output format the filter instance is processing.
    Miro::ImageFormatIDL outputFormat_;
    //! TODO: Implementation. Currently allways false.
    bool interfaceAllowed_;
    //! TODO: Implementation. Currently allways false.
    bool inplace_;
    //! The filter instances parameters.
    FilterParameters const * params_;

    //! Image recording timestamp
    ACE_Time_Value const& imageTimeStamp() const;

    //! Filter processing time statistics.
    /**
     * Used for performance monitoring via the VideoBroker interface.
     */
    Miro::TimeSeries<100> timeFilter_;
    //! Filter tree processing time statistics.
    /**
     * Used for performance monitoring via the VideoBroker interface.
     */
    Miro::TimeSeries<100> timeFilterTree_;

    //! Pointer to the buffer manager instance;
    BufferManager * bufferManager_;

    //! The index of the input buffer in the predecessors buffer manager.
    unsigned long inputBufferIndex_;
    //! Pointer to the input buffer.
    unsigned char const * inputBuffer_;
    //! Pointer to filter meta data of the input buffer.
    FilterImageParameters const * inputParameters_;
    //! Index of the output buffer in the own buffer manager.
    unsigned long outputBufferIndex_;
    //! Pointer to the output buffer.
    unsigned char * outputBuffer_;
    //! Pointer to filter meta data for the output buffer.
    FilterImageParameters * outputParameters_;
    //! Pointer the video interface.
    /** NULL if no interface is available. */
    Miro::VideoImpl * interface_;

    //! Filter predecessor.
    /** NULL for root filters. */
    Filter * pre_;
    //! Filter predecessor links.
    /** 
     * Filters are allowed to have additional input filters, that
     * reside higher in the filter tree. Those are called filter
     * links, like soft links in a file system.
     */
    FilterPreVector preLink_;

    //! Filter successors.
    /** 
     * A filter can have multiple successor filters, recursively
     * spanning a filter tree.
     */
    FilterVector succ_;
    //! Filter successor links.
    /** 
     * Filters are allowed to have additional input filters, that
     * reside higher in the filter tree. Those are called filter
     * links, like soft links in a file system.
     */
    FilterSuccVector succLink_;

    //! Filter successor links.
    /** 
     * Filters are allowed to have additional input filters, that
     * reside higher in the filter tree. Those are called filter
     * links, like soft links in a file system.
     */
    AsynchSuccVector asynchSuccLink_;

    //! Manager for asynchronous filter links.
    AsynchLinkManager asynchLinkManager_;

    //! VideoBroker request queue.
    BrokerLinkVector brokerLink_;

    // Connection management.

    //! Number of successor and client connections (recursive).
    unsigned int connections_;
    //! Result of a calcConnectivity run for the filter instance.
    bool connected_;
    //! Number of successors (plain).
    unsigned int successors_;

    //! Name of the filter.
    std::string name_;
    //! Number of the filter.
    unsigned int number_;

    //--------------------------------------------------------------------------
    // protected static data
    //--------------------------------------------------------------------------

    //! Connection management lock.
    static Miro::Mutex connectionMutex_;
    static Miro::Condition connectionCondition_;

  private:
    //--------------------------------------------------------------------------
    // private methods
    //--------------------------------------------------------------------------

    //! Private copy constructor to prevent accidental usage.
    Filter(const Filter&) {}
    //! Private assignement operator to prevent accidental usage.
    Filter const & operator = (Filter const&) { return *this; }

    //--------------------------------------------------------------------------
    // friend declarations
    //--------------------------------------------------------------------------

    friend class Consumer;
    friend class AsynchBufferSet;
    friend class Miro::VideoImpl;
  };
}

#include "VideoFilter.i"

#endif // Video_Filter_h
