Pod::Spec.new do |s|
  s.name         = "ZMBLESDK"
  s.version      = "1.2.0"
  s.summary      = "Bluetooth"
  s.homepage     = "https://github.com/humoroutlaw/ZMBLESDK"
  s.license      = { :type => 'MIT' } 
  s.author    = "humoroutlaw"
  s.platform     = :ios, "9.0"
  s.source  = {:git => "https://github.com/humoroutlaw/ZMBLESDK.git", :tag => s.version}
  s.vendored_frameworks = 'ZMBLESDK/ZMBLESDK.framework'
  s.requires_arc = true
end
