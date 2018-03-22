#ifndef HITBOX_BUILDER_HITBOX_MANAGER_HPP
#define HITBOX_BUILDER_HITBOX_MANAGER_HPP

#include <cassert>
#include <unordered_map>

#include "detail/BoundingBoxBuilder.hpp"
#include "detail/ContourBuilder.hpp"
#include "detail/PolygonBuilder.hpp"
#include "detail/PolygonPartitioner.hpp"

#include "Types.hpp"

namespace Hitbox {

template <typename Identifier>
class Manager {
 public:
  void load(const Identifier& id, const sf::Sprite& sprite, size_t accuracy) {
    auto contour = _contourBuilder.make(sprite);
    auto polygon = _polygonBuilder.make(contour, accuracy);
    auto boundingBox = _boundingBoxBuilder.make(contour);
    auto polygons = _polygonPartitioner.make(std::move(polygon));

    auto hitbox = Hitbox(std::move(polygons), std::move(boundingBox));
    _hitboxes[id] = std::move(hitbox);
  }

  const Hitbox& get(const Identifier& id) const {
    auto found = _hitboxes.find(id);
    assert(found != _hitboxes.end());

    return found->second;
  }

 private:
  Detail::ContourBuilder _contourBuilder;
  Detail::PolygonBuilder _polygonBuilder;
  Detail::BoundingBoxBuilder _boundingBoxBuilder;
  Detail::PolygonPartitioner _polygonPartitioner;

 private:
  std::unordered_map<Identifier, Hitbox> _hitboxes;
};

} /* namespace Hitbox */

#endif
