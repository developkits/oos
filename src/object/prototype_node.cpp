/*
 * This file is part of OpenObjectStore OOS.
 *
 * OpenObjectStore OOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenObjectStore OOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenObjectStore OOS. If not, see <http://www.gnu.org/licenses/>.
 */

#include "object/prototype_node.hpp"
#include "object/object_store.hpp"
#include "object/object_proxy.hpp"

using namespace std;

namespace oos {

prototype_node::prototype_node()
  : parent(0)
  , prev(0)
  , next(0)
  , first(0)
  , last(0)
  , producer(0)
  , op_first(0)
  , op_marker(0)
  , op_last(0)
  , depth(0)
  , count(0)
  , abstract(false)
  , initialized(false)
{
}

prototype_node::prototype_node(object_base_producer *p, const char *t, bool a)
  : parent(0)
  , prev(0)
  , next(0)
  , first(new prototype_node)
  , last(new prototype_node)
  , producer(p)
  , op_first(0)
  , op_marker(0)
  , op_last(0)
  , depth(0)
  , count(0)
  , type(t)
  , abstract(a)
  , initialized(false)
{
  first->next = last;
  last->prev = first;
//  aliases.insert(t);
}

void prototype_node::initialize(object_base_producer *p, const char *t, bool a)
{
  first = new prototype_node;
  last = new prototype_node;
  producer = p;
  type.assign(t);
  abstract = a;
//  aliases.insert(t);
  initialized = true;
  first->next = last;
  last->prev = first;  
}

prototype_node::~prototype_node()
{
  if (first) {
    delete first;
  }
  if (last) {
    delete last;
  }
  if (producer) {
    delete producer;
  }
}

void
prototype_node::clear()
{
  if (empty(true)) {
    return;
  }
  // remove object proxies until first and marker are left
  // adjust marker first
  
  adjust_left_marker(op_first->next, op_marker);
  adjust_right_marker(op_marker->prev, op_first);

  while (op_first->next != op_marker) {
    object_proxy *op = op_first->next;
    // remove object proxy from list
    op->unlink();
    // delete object proxy and object
    delete op;
  }
  count = 0;
}

bool
prototype_node::empty(bool self) const
{
  return op_first->next == (self ? op_marker : op_last);
}

unsigned long
prototype_node::size() const
{
  return count;
}

void
prototype_node::insert(prototype_node *child)
{
  child->parent = this;
  child->prev = last->prev;
  child->next = last;
  last->prev->next = child;
  last->prev = child;
  // set depth
  child->depth = depth + 1;
  // set object proxy pointer
  // 1. first
  if (op_first->next == op_last) {
    // node hasn't any object (proxy)
    child->op_first = op_first;
  } else {
    // node has some objects (proxy)
    child->op_first = op_last->prev;
  }
  // 2. marker
  child->op_marker = op_last;
  // 3. last
  child->op_last = op_last;
}    

void prototype_node::remove()
{
  // delete all object proxies
  clear();
  // delete all cild nodes
  while (first->next != last) {
    prototype_node *node = first->next;
    node->remove();
    delete node;
  }
  // unlink this node
  unlink();
}

void prototype_node::unlink()
{
  // unlink node
  prev->next = next;
  next->prev = prev;
  next = 0;
  prev = 0;
}

prototype_node* prototype_node::next_node() const
{
  // if we have a child, child is the next iterator to return
  // (if we don't do iterate over the siblings)
  if (first->next != last)
    return first->next;
  else {
    // if there is no child, we check for sibling
    // if there is a sibling, this is our next iterator to return
    // if not, we go back to the parent
    const prototype_node *node = this;
    while (node->parent && node->next == node->parent->last) {
      node = node->parent;
    }
    return node->next;
  }
}

prototype_node* prototype_node::previous_node() const
{
  // if node has a previous sibling, we set it
  // as our next iterator. then we check if there
  // are last childs. if so, we set the last last
  // child as our iterator
  if (prev && prev->prev) {
    const prototype_node *node = prev;
    while (node->last && node->first->next != node->last) {
      node = node->last->prev;
    }
    return const_cast<prototype_node*>(node);
    // if there is no previous sibling, our next iterator
    // is the parent of the node
  } else {
    return parent;
  }
}

bool prototype_node::is_child_of(const prototype_node *parent) const
{
  const prototype_node *node = this;
  while (parent->depth < node->depth) {
    node = node->parent;
  }
  return node == parent;
}

/*
 * adjust the marker of all predeccessor nodes
 * self and last marker
 */
void prototype_node::adjust_left_marker(object_proxy *old_proxy, object_proxy *new_proxy)
{
  // store start node
  prototype_node *node = this;
  // get previous node
  node = node->previous_node();
  while (node) {
    if (node->op_marker == old_proxy) {
      node->op_marker = new_proxy;
    }
    if (node->depth >= depth && node->op_last == old_proxy) {
      node->op_last = new_proxy;
    }
    node = node->previous_node();
  }
}

void prototype_node::adjust_right_marker(object_proxy* old_proxy, object_proxy *new_proxy)
{
  // store start node
  prototype_node *node = this;
  // get previous node
  node = node->next_node();
  //bool first = true;
  while (node) {
    if (node->op_first == old_proxy) {
      node->op_first = new_proxy;
    }
    node = node->next_node();
  }
}

std::ostream& operator <<(std::ostream &os, const prototype_node &pn)
{
  if (pn.parent) {
    os << "\t" << pn.parent->type << " -> " << pn.type << "\n";
  }
  os << "\t" << pn.type << " [label=\"{" << pn.type << " (" << &pn << ")";
  os << "|{op_first|" << pn.op_first << "}";
  os << "|{op_marker|" << pn.op_marker << "}";
  os << "|{op_last|" << pn.op_last << "}";
  os << "|{parent|" << pn.parent << "}";
  os << "|{prev|" << pn.prev << "}";
  os << "|{next|" << pn.next << "}";
  os << "|{first|" << pn.first << "}";
  os << "|{last|" << pn.last << "}";
  // determine size
  int i = 0;
  object_proxy *iop = pn.op_first;
  while (iop && iop->next != pn.op_marker) {
    ++i;
    iop = iop->next;
  }
  os << "|{size|" << i << "}";
  /*
  os << "|{relations}";
  // list relations
  prototype_node::type_map_t::const_iterator first = pn.relations.begin();
  prototype_node::type_map_t::const_iterator last = pn.relations.end();
  while (first != last) {
    prototype_node::field_prototype_map_t::const_iterator ffirst = first->second.begin();
    prototype_node::field_prototype_map_t::const_iterator llast = first->second.end();
    while (ffirst != llast) {
      os << "|{" << (ffirst->second ? ffirst->second->type : "null") << "|" << ffirst->first << "}";
      ++ffirst;
    }
    ++first;
  }
  */
  os << "}\"]\n";
  return os;
}

}
