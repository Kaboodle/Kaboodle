#
# Be sure to run `pod spec lint NAME.podspec' to ensure this is a
# valid spec and remove all comments before submitting the spec.
#
# To learn more about the attributes see http://docs.cocoapods.org/specification.html
#
Pod::Spec.new do |s|
  s.name         = "Kaboodle"
  s.version      = "0.1.0"
  s.summary      = "The Kaboodle Framework is a library of drop-in replacements for Cocoa Touch classes"
  s.homepage     = "https://github.com/Kaboodle/Kaboodle"
  s.license      = 'MIT'
  s.author       = { "Kaboodle Framework" => "kaboodleframework@gmail.com" }
  s.source       = { :git => "https://github.com/Kaboodle/Kaboodle.git", :tag => s.version.to_s }

  s.platform     = :ios
  s.ios.deployment_target = '6.0'
  s.requires_arc = true

  s.resources = 'Assets'

  s.subspec 'UIKaboodle' do |uiks|
    uiks.source_files = 'Classes/UIKaboodle'
    uiks.private_header_files = 'Classes/UIKaboodle/Private'
    uiks.header_dir = 'UIKaboodle'
    uiks.frameworks = 'UIKit'
  end
end
