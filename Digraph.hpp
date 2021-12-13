// Digraph.hpp
//
// ICS 46 Winter 2021
// Project #5: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph.  The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <string>
#include <functional>
#include <queue>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>



// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}



// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};





// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the precedessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> m;
    std::vector<int> v;
    std::vector<std::pair<int, int>> e;
    int vertexC;
    int edgeC;

    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.

    // if vertex doesn't exist, throw DigraphException
    void VertexExist(int vertex) const;

    //return the iterator point to the vertex in the std::map
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator findVertex(int vertex);

    //if the vertex exists, throw exception
    void findVertexNotExist(int vertex);

    //a helper function for isStronglyConnected
    void DFTr(int vertex, std::vector<int>& vertices) const;

    //return the index of a vertex in an vector
    int indexOf(int vertex, std::vector<int>& vertices) const;

};



// You'll need to implement the member functions below.  There's enough
// code in place to make them compile, but they'll all need to do the
// correct thing instead.

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
    :vertexC{0}, edgeC{0}
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
    :vertexC{0}, edgeC{0}
{
    for(int n: d.v)
    {
        addVertex(n, d.m.at(n).vinfo);
    }
    for(int n: d.v)
    {
        for(auto edge: d.m.at(n).edges)
        {
            addEdge(edge.fromVertex, edge.toVertex, edge.einfo);
        }
    }
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
{
    vertexC = std::move(d.vertexC);
    edgeC = std::move(d.edgeC);
    v = std::move(d.v);
    e = std::move(d.e);
    m = std::move(d.m);
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    if(this != &d)
    {
        vertexC = 0;
        edgeC = 0;
        std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> new_m{};
        std::vector<int> new_v{};
        std::vector<std::pair<int, int>> new_e{};
        m = new_m;
        v = new_v;
        e = new_e;
        for(int n: d.v)
        {
            addVertex(n, d.m.at(n).vinfo);
        }
        for(int n: d.v)
        {
            for(auto edge: d.m.at(n).edges)
            {
                addEdge(edge.fromVertex, edge.toVertex, edge.einfo);
            }
        }
    }
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
    if(this != &d)
    {
        std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> new_m{};
        std::vector<int> new_v{};
        std::vector<std::pair<int, int>> new_e{};
        m = new_m;
        v = new_v;
        e = new_e;
        vertexC = std::move(d.vertexC);
        edgeC = std::move(d.edgeC);
        v = std::move(d.v);
        e = std::move(d.e);
        m = std::move(d.m);
    }
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{

    return v;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    return e;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
    std::vector<std::pair<int, int>> result;
    VertexExist(vertex);
    for(auto const& it: m.at(vertex).edges)
    {
        result.push_back(std::make_pair(it.fromVertex, it.toVertex));
    }
    return result;
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    VertexExist(vertex);
    return m.at(vertex).vinfo;
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
    VertexExist(fromVertex);
    VertexExist(toVertex);
    for(auto const& it: m.at(fromVertex).edges)
    {
        if(it.toVertex == toVertex)
        {
            return it.einfo;
        }
    }
    throw DigraphException("No such edge exist");
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    findVertexNotExist(vertex);
    vertexC++;
    v.push_back(vertex);
    m[vertex] = DigraphVertex<VertexInfo, EdgeInfo>{vinfo, {}};
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{

    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator i;
    i = findVertex(fromVertex);
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator j;
    j = findVertex(toVertex);
    for (typename std::list<DigraphEdge<EdgeInfo>>::iterator it = i->second.edges.begin(); it != i->second.edges.end(); it++)
    {
        if(it->toVertex == toVertex)
        {
            throw DigraphException("Edge exist");
        }
    }
    edgeC++;
    e.push_back(std::make_pair(fromVertex, toVertex));
    i->second.edges.push_back(DigraphEdge<EdgeInfo>{fromVertex, toVertex, einfo});
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    std::vector<int> v_;
    std::vector<std::pair<int, int>> e_;
    for(int ve: v)
    {
        if(ve != vertex)
        {
            v_.push_back(ve);
        }
    }
    auto m_ = m;
    vertexC = 0;
    edgeC = 0;
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> new_m{};
    std::vector<int> new_v{};
    std::vector<std::pair<int, int>> new_e{};
    m = new_m;
    v = new_v;
    e = new_e;
    for(int n: v_)
    {
        addVertex(n, m_.at(n).vinfo);
    }
    for(int n: v_)
    {
        for(auto edge: m_.at(n).edges)
        {
            if(edge.fromVertex != vertex && edge.toVertex != vertex)
            {
                addEdge(edge.fromVertex, edge.toVertex, edge.einfo);
            }
        }
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    edgeInfo(fromVertex, toVertex);
    edgeC--;
    std::list<DigraphEdge<EdgeInfo>> edges_;
    for(auto i: m.at(fromVertex).edges)
    {
        if(i.toVertex != toVertex)
        {
            edges_.push_back(DigraphEdge<EdgeInfo>{i.fromVertex, i.toVertex, i.einfo});
        }
    }
    m.at(fromVertex).edges = edges_;
    typename std::vector<std::pair<int, int>>::iterator it;
    it = std::find(e.begin(), e.end(), std::make_pair(fromVertex, toVertex));
    e.erase(it);
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return vertexC;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    return edgeC;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    VertexExist(vertex);
    return m.at(vertex).edges.size();
}


template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    bool result = true;
    for(int vertex: v)
    {
        std::vector<int> vertices = v;
        DFTr(vertex, vertices);
        if (vertices.size() != 0)
        {
            result = false;
            break;
        }
    }
    return result;
}


template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
    std::vector<bool> Kv (v.size(), false);
    std::map<int, int> Pv;
    for (auto vertex: v)
    {
        Pv[vertex] = vertex;
    }
    std::vector<double> Dv (v.size(), std::numeric_limits<double>::max());
    std::vector<int> vertices = v;

    int startIndex = indexOf(startVertex, vertices);
    Dv[startIndex] = 0;

    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> pq;
    pq.push(std::make_pair(0, startVertex));
    while (!pq.empty())
    {
        int vertex = pq.top().second;
        pq.pop();

        int index = indexOf(vertex, vertices);
        
        if(Kv[index] == false)
        {
            Kv[index] = true;
            for(auto edge: m.at(vertex).edges)
            {
                int w = edge.toVertex;
                int indexW = indexOf(w, vertices);
                if(Dv[indexW] > Dv[index] + edgeWeightFunc(edgeInfo(vertex, w)))
                {
                    Dv[indexW] = Dv[index] + edgeWeightFunc(edgeInfo(vertex, w));
                    Pv[w] = vertex;
                    pq.push(std::make_pair(Dv[indexW], w));
                }
            }
        }
    }
    return Pv;
}

template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::VertexExist(int vertex) const
{
    int r = -1;
    for(int i = 0; i < v.size(); i++)
    {
        if(v[i] == vertex)
        {
            r = i;
        }
    }
    if(r == -1)
    {
        throw DigraphException("Vertex " + std::to_string(vertex) + "not exist");
    }
}


template <typename VertexInfo, typename EdgeInfo>
typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator Digraph<VertexInfo, EdgeInfo>::findVertex(int vertex)
{
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::iterator i;
    i = m.find(vertex);
    if (i == m.end())
    {
        throw DigraphException("Vertex " + std::to_string(vertex) + "not exist");
    }
    return i;
}



template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::findVertexNotExist(int vertex)
{
    typename std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>::const_iterator i;
    i = m.find(vertex);
    if (i != m.end())
    {
        throw DigraphException("Vertex " + std::to_string(vertex) + " exist");
    }
}

template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::DFTr(int vertex, std::vector<int>& vertices) const
{
    int index = -1;
    for(int i = 0; i < vertices.size(); i++)
    {
        if(vertices.at(i) == vertex)
        {
            index = i;
            break;
        }
    }
    vertices.erase(vertices.begin() + index);
    for(auto edge: m.at(vertex).edges)
    {
        int index = -1;
        for(int i = 0; i < vertices.size(); i++)
        {
            if(vertices.at(i) == edge.toVertex)
            {
                index = i;
                break;
            }
        }
        if(index != -1)
        {
            DFTr(edge.toVertex, vertices);
        }
    }
}

template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::indexOf(int vertex, std::vector<int>& vertices) const
{
    int index = -1;
    for(int i = 0; i < vertices.size(); i++)
    {
        if(vertices.at(i) == vertex)
        {
            index = i;
            break;
        }
    }
    return index;
}

#endif // DIGRAPH_HPP

