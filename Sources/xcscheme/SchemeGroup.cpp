// Copyright 2013-present Facebook. All Rights Reserved.

#include <xcscheme/SchemeGroup.h>

using xcscheme::SchemeGroup;
using xcscheme::XC::Scheme;
using libutil::FSUtil;
using libutil::SysUtil;

SchemeGroup::SchemeGroup()
{
}

SchemeGroup::shared_ptr SchemeGroup::
Open(std::string const &path, std::string const &name)
{
    if (path.empty()) {
        errno = EINVAL;
        return nullptr;
    }

    if (!FSUtil::TestForDirectory(path)) {
        return nullptr;
    }

    std::string realPath = FSUtil::ResolvePath(path);
    if (realPath.empty()) {
        return nullptr;
    }

    SchemeGroup::shared_ptr group = std::make_shared <SchemeGroup> ();
    group->_path = path;
    group->_name = name;

    std::string schemePath;

    schemePath = path + "/xcshareddata/xcschemes";
    FSUtil::EnumerateDirectory(schemePath, "*.xcscheme",
            [&](std::string const &filename) -> bool
            {
                std::string name = filename.substr(0, filename.find('.'));
                auto scheme = Scheme::Open(name, std::string(), schemePath + "/" + filename);
                if (!scheme) {
                    fprintf(stderr, "warning: failed parsing shared scheme '%s'\n", name.c_str());
                } else {
                    group->_schemes.push_back(scheme);
                }

                if (!group->_defaultScheme && name == group->name()) {
                    group->_defaultScheme = scheme;
                }
                return true;
            });

    std::string userName = SysUtil::GetUserName();
    if (!userName.empty()) {
        schemePath = path + "/xcuserdata/" + userName + ".xcuserdatad/xcschemes";
        FSUtil::EnumerateDirectory(schemePath, "*.xcscheme",
                [&](std::string const &filename) -> bool
                {
                    std::string name = filename.substr(0, filename.find('.'));
                    auto scheme = Scheme::Open(name, userName, schemePath + "/" + filename);
                    if (!scheme) {
                        fprintf(stderr, "warning: failed parsing user scheme '%s'\n", name.c_str());
                    } else {
                        group->_schemes.push_back(scheme);
                    }

                    if (!group->_defaultScheme && name == group->name()) {
                        group->_defaultScheme = scheme;
                    }
                    return true;
                });
    }

    if (!group->_schemes.empty() && !group->_defaultScheme) {
        group->_defaultScheme = group->_schemes[0];
    }

    return group;
}