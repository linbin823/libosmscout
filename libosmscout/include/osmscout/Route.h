#ifndef OSMSCOUT_ROUTE_H
#define OSMSCOUT_ROUTE_H

/*
  This source is part of the libosmscout library
  Copyright (C) 2009  Tim Teulings

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

#include <list>

#include <osmscout/TypeConfig.h>

namespace osmscout {

  /**
   * Description of a route, enhanced with information that are required to
   * give a human textual (or narrative) drive instructions;
   *
   * A route consists of nodes. A Node can be the crossing point of a number of
   * ways and is a route decision point (where the driver possibly has the change ways)
   * that requires some potential action by the driver.
   *
   * For each node you can pass a number of descriptions. For the way from the current node
   * to the next node also a number of descriptions can get retrieved.
   *
   * Descriptions are typed and must derive from class Description..
   */
  class OSMSCOUT_API RouteDescription
  {
  public:
    /** Constant for a description of the start node (StartDescription) */
    static const char* const NODE_START_DESC;
    /** Constant for a description of the target node (TargetDescription) */
    static const char* const NODE_TARGET_DESC;
    /** Constant for a description of name of the way (NameDescription) */
    static const char* const WAY_NAME_DESC;
    /** Constant for a description of a change of way name (NameChangedDescription) */
    static const char* const WAY_NAME_CHANGED_DESC;
    /** Constant for a description of list of way name crossing a node (CrossingWaysDescription) */
    static const char* const CROSSING_WAYS_DESC;

  public:
    /*
     * Base class of all descriptions.
     */
    class OSMSCOUT_API Description : public Referencable
    {
    public:
      virtual~Description();
    };

    typedef Ref<Description> DescriptionRef;

    /**
     * Start of the route
     */
    class OSMSCOUT_API StartDescription : public Description
    {
    private:
      std::string description;

    public:
      StartDescription(const std::string& description);

      std::string GetDescription() const;
    };

    typedef Ref<StartDescription> StartDescriptionRef;

    /**
     * Start of the route
     */
    class OSMSCOUT_API TargetDescription : public Description
    {
    private:
      std::string description;

    public:
      TargetDescription(const std::string& description);

      std::string GetDescription() const;
    };

    typedef Ref<TargetDescription> TargetDescriptionRef;

    /**
     * Something has a name. A name consists of a name and a optional alphanumeric
     * reference (LIke B1 or A40).
     */
    class OSMSCOUT_API NameDescription : public Description
    {
    private:
      std::string name;
      std::string ref;

    public:
      NameDescription(const std::string& name,
                      const std::string& ref);

      std::string GetName() const;
      std::string GetRef() const;
    };

    typedef Ref<NameDescription> NameDescriptionRef;

    /**
     * Something has a name. A name consists of a name and a optional alphanumeric
     * reference (Like B1 or A40).
     */
    class OSMSCOUT_API NameChangedDescription : public Description
    {
      NameDescriptionRef originDescription;
      NameDescriptionRef targetDescription;

    public:
      NameChangedDescription(NameDescription* originDescription,
                             NameDescription* targetDescription);

      inline const NameDescriptionRef& GetOriginDesccription() const
      {
        return originDescription;
      }

      inline const NameDescriptionRef& GetTargetDesccription() const
      {
        return targetDescription;
      }
    };

    typedef Ref<NameChangedDescription> NameChangedDescriptionRef;

    /**
     * List the names of allways, that are crossing the current node.
     */
    class OSMSCOUT_API CrossingWaysDescription : public Description
    {
    public:
      enum Type {
        normal,
        roundaboutEnter,
        roundaboutLeave
      };

    private:
      Type                          type;
      NameDescriptionRef            originDescription;
      NameDescriptionRef            targetDescription;
      std::list<NameDescriptionRef> descriptions;

    public:
      CrossingWaysDescription(Type type,
                              NameDescription* originDescription,
                              NameDescription* targetDescription);

      void AddDescription(NameDescription* description);

      inline Type GetType() const
      {
        return type;
      }

      inline const NameDescriptionRef& GetOriginDesccription() const
      {
        return originDescription;
      }

      inline const NameDescriptionRef& GetTargetDesccription() const
      {
        return targetDescription;
      }

      inline const std::list<NameDescriptionRef>& GetDescriptions() const
      {
        return descriptions;
      }
    };

    typedef Ref<CrossingWaysDescription> CrossingWaysDescriptionRef;

    class Node
    {
    private:
      Id                                   currentNodeId;
      std::vector<Id>                      ways;
      Id                                   pathWayId;
      Id                                   targetNodeId;
      double                               distance;
      double                               time;
      std::map<std::string,DescriptionRef> descriptions;

    public:
      Node(Id currentNodeId,
           const std::vector<Id>& ways,
           Id pathWayId,
           Id targetNodeId);

      inline Id GetCurrentNodeId() const
      {
        return currentNodeId;
      }

      inline const std::vector<Id>& GetWays() const
      {
        return ways;
      }

      inline Id GetPathWayId() const
      {
        return pathWayId;
      }

      inline Id GetTargetNodeId() const
      {
        return targetNodeId;
      }

      /**
       * Distance from the start of the route in km.
       */
      inline double GetDistance() const
      {
        return distance;
      }

      /**
       * Time from the start of the route in h.
       */
      inline double GetTime() const
      {
        return time;
      }

      bool HasDescription(const char* name) const;
      DescriptionRef GetDescription(const char* name) const;

      void SetDistance(double distance);
      void SetTime(double time);

      void AddDescription(const char* name, Description* description);
    };

  private:
    std::list<Node> nodes;

  public:
    RouteDescription();

    void Clear();

    void AddNode(Id currentNodeId,
                 const std::vector<Id>& ways,
                 Id pathWayId,
                 Id targetNodeId);

    inline std::list<Node>& Nodes()
    {
      return nodes;
    }
  };
}

#endif
