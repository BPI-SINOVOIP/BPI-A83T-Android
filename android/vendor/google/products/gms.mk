###############################################################################
# GMS mandatory core packages
PRODUCT_PACKAGES += \
    ConfigUpdater \
    GoogleBackupTransport \
    GoogleFeedback \
    GoogleLoginService \
    GoogleOneTimeInitializer \
    GooglePartnerSetup \
    GoogleServicesFramework \
    GoogleCalendarSyncAdapter \
    GoogleContactsSyncAdapter \
    GmsCore \
    Phonesky \
    SetupWizard

PRODUCT_PACKAGES += \
    google.xml \
    google_generic_update.txt \
    com.google.android.maps.jar \
    com.google.android.media.effects.jar

# Overlay For GMS devices
PRODUCT_PACKAGE_OVERLAYS := vendor/google/products/gms_overlay

# Setting PRODUCT_PREBUILT_WEBVIEWCHROMIUM as yes will prevent from building
# webviewchromium from source, and use prebuilt WebViewGoogle.apk in GMS pack
PRODUCT_PREBUILT_WEBVIEWCHROMIUM := yes
#
ifeq ($(PRODUCT_PREBUILT_WEBVIEWCHROMIUM),yes)
PRODUCT_PACKAGES += WebViewGoogle
# The following framework overlay must be included if prebuilt WebViewGoogle.apk is used
PRODUCT_PACKAGE_OVERLAYS += vendor/google/apps/WebViewGoogle/overlay
endif

# Chrome browser selection
# By default, Chrome will be the only preloaded system browser
# Use ChromeWithBrowser, if Chrome is preloaded along with another browser side-by-sde
PRODUCT_PACKAGES += Chrome
#PRODUCT_PACKAGES += ChromeWithBrowser
#
# Uncomment the following line if customized homepage provider for Chrome should be installed
# For the details, see Android.mk in apps/Chrome directory
#PRODUCT_PACKAGES += ChromeCustomizations

# GMS mandatory application packages
PRODUCT_PACKAGES += \
    Books \
    Drive \
    Gmail2 \
    Hangouts \
    Maps \
    Music2 \
    Newsstand \
    PlayGames \
    PlusOne \
    Street \
    Velvet \
    Videos \
    YouTube

# GMS optional application packages
PRODUCT_PACKAGES += \
    AndroidForWork \
    CalendarGoogle \
    CloudPrint \
    DeskClockGoogle \
    EditorsDocs \
    EditorsSheets \
    EditorsSlides \
    FaceLock \
    GoogleCamera \
    GoogleHome \
    GoogleTTS \
    Keep \
    LatinImeGoogle \
    NewsWeather \
    TagGoogle \
    talkback

# more GMS optional application packages
PRODUCT_PACKAGES += \
    DMAgent \
    GoogleEars \
    GoogleEarth \
    GoogleHindiIME \
    GooglePinyinIME \
    KoreanIME \
    Wallet

# Add overlay for ManagedProvisioning if this devices targets AfW
ifneq ($(filter $(PRODUCT_PACKAGES),AndroidForWork),)
# The following framework overlay must be included if AndroidForWork.apk is included
PRODUCT_PACKAGE_OVERLAYS += vendor/google/apps/AndroidForWork/overlay
endif

# Overlay for Google network and fused location providers
$(call inherit-product, device/sample/products/location_overlay.mk)

# Overrides
PRODUCT_PROPERTY_OVERRIDES += \
    ro.setupwizard.mode=OPTIONAL \
    ro.com.google.gmsversion=5.1_r1
