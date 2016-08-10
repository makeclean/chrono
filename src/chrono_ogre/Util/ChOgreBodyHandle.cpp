#include "ChOgreBodyHandle.h"

namespace chrono{
namespace ChOgre {

ChOgreBodyHandle::ChOgreBodyHandle() {}

ChOgreBodyHandle::ChOgreBodyHandle(const ChOgreBodyHandle& other) {
    m_pBody = other.m_pBody;
}

ChOgreBodyHandle::ChOgreBodyHandle(ChOgreBodyHandle&& other) {
    m_pBody = std::move(other.m_pBody);
}

ChOgreBodyHandle::ChOgreBodyHandle(ChOgreBody& Body) {
    m_pBody = ChOgreBodySharedPtr(&Body);
}

ChOgreBodyHandle::ChOgreBodyHandle(ChOgreBodySharedPtr& BodyPtr) {
    m_pBody = BodyPtr;
}

ChOgreBodyHandle::~ChOgreBodyHandle() {}

ChOgreBodyHandle& ChOgreBodyHandle::operator=(const ChOgreBodyHandle& other) {
    if (this != &other) {
        m_pBody = other.m_pBody;
    }

    return *this;
}

ChOgreBodyHandle& ChOgreBodyHandle::operator=(ChOgreBodyHandle&& other) {
    if (this != &other) {
        m_pBody = std::move(other.m_pBody);
    }

    return *this;
}

std::shared_ptr<ChBody> ChOgreBodyHandle::operator->() {
    return m_pBody->getChBody();
}

std::shared_ptr<ChBody> ChOgreBodyHandle::getChBody() {
    return m_pBody->getChBody();
}

ChOgreBody& ChOgreBodyHandle::body() {
    return *m_pBody;
}

void ChOgreBodyHandle::setBodyPtr(ChOgreBodySharedPtr& BodyPtr) {
    m_pBody = BodyPtr;
}
}
}
