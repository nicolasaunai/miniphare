TEMPLATE = subdirs

#SUBDIRS += GridLayout/gridlayout.pro \
#           Plasma/plasma.pro \
#           Faraday/faraday.pro \
#           gtest/gtest.pro

SUBDIRS += gtest/gtest.pro\
           gmock/gmock.pro \
           GridLayout/gridlayout.pro\
           Plasma/plasma.pro \
      #     faraday


#gtest.subdirs      = gtest/gtest.pro
#gmock.subdirs      = gmock/gmock.pro
#gridlayout.subdirs = GridLayout/gridlayout.pro
#plasma.SUBDIRS     = Plasma/plasma.pro
#faraday.subdirs    = Faraday/faraday.pro


CONFIG += ordered

