/*
 * GDevelop JS Platform
 * Copyright 2008-2016 Florian Rival (Florian.Rival@gmail.com). All rights
 * reserved. This project is released under the MIT License.
 */
#ifndef EVENTSCODEGENERATOR_H
#define EVENTSCODEGENERATOR_H
#include <set>
#include <string>
#include <vector>
#include "GDCore/Events/CodeGeneration/EventsCodeGenerator.h"
#include "GDCore/Events/Event.h"
#include "GDCore/Events/InstructionsList.h"
namespace gd {
class ClassWithObjects;
class ObjectMetadata;
class BehaviorMetadata;
class InstructionMetadata;
class ExpressionCodeGenerationInformation;
class EventsCodeGenerationContext;
}  // namespace gd

namespace gdjs {

/**
 * \brief The class being responsible for generating Javascript code from
 * events.
 *
 * See also gd::EventsCodeGenerator.
 */
class EventsCodeGenerator : public gd::EventsCodeGenerator {
 public:
  /**
   * Generate complete JS file for executing events of a scene
   *
   * \param project Project used
   * \param scene Scene used
   * \param events events of the scene
   * \param includeFiles A reference to a set of strings where needed
   * includes files will be stored.
   * \param compilationForRuntime Set this to true if the code is generated for
   * runtime.
   *
   * \return JavaScript code
   */
  static gd::String GenerateSceneEventsCompleteCode(
      gd::Project& project,
      const gd::Layout& scene,
      const gd::EventsList& events,
      std::set<gd::String>& includeFiles,
      bool compilationForRuntime = false);

  /**
   * Generate JavaScript for executing events in a function
   *
   * \param project Project used
   * \param parameters The parameters of the function (objects will be deduced from these).
   * \param events events of the scene
   * \param compilationForRuntime Set this to true if the code is generated for
   * runtime.
   *
   * \return JavaScript code
   */
  static gd::String GenerateEventsFunctionCode(
      gd::Project& project,
      const std::vector<gd::ParameterMetadata> & parameters,
      const gd::EventsList& events,
      bool compilationForRuntime = false);

  /**
   * \brief Generate code for executing an event list
   * \note To reduce the stress on JS engines, the code is generated inside
   * a separate JS function (see
   * gd::EventsCodeGenerator::AddCustomCodeOutsideMain). This method will return
   * the code to call this separate function.
   *
   * \param events std::vector of events
   * \param context Context used for generation
   * \return Code
   */
  virtual gd::String GenerateEventsListCode(
      gd::EventsList& events, const gd::EventsCodeGenerationContext& context);

  /**
   * Generate code for executing a condition list
   *
   * \param game Game used
   * \param scene Scene used
   * \param conditions std::vector of conditions
   * \param context Context used for generation
   * \return JS code.
   */
  virtual gd::String GenerateConditionsListCode(
      gd::InstructionsList& conditions,
      gd::EventsCodeGenerationContext& context);

  /**
   * \brief Generate the full name for accessing to a boolean variable used for
   * conditions.
   */
  virtual gd::String GenerateBooleanFullName(
      const gd::String& boolName,
      const gd::EventsCodeGenerationContext& context);

  /**
   * \brief Set a boolean to false.
   */
  virtual gd::String GenerateBooleanInitializationToFalse(
      const gd::String& boolName,
      const gd::EventsCodeGenerationContext& context);

  /**
   * \brief Get the full name for accessing to a list of objects
   */
  virtual gd::String GetObjectListName(
      const gd::String& name, const gd::EventsCodeGenerationContext& context);

  /**
   * \brief Get the namespace to be used to store code generated
   * objects/values/functions, with the extra "dot" at the end to be used to
   * access to a property/member.
   *
   * Example: "gdjs.something."
   */
  virtual gd::String GetCodeNamespaceAccessor() {
    return GetCodeNamespace() + ".";
  };

  /**
   * \brief Get the namespace to be used to store code generated
   * objects/values/functions.
   *
   * Example: "gdjs.something"
   */
  virtual gd::String GetCodeNamespace();

 protected:
  virtual gd::String GenerateParameterCodes(
      const gd::String& parameter,
      const gd::ParameterMetadata& metadata,
      gd::EventsCodeGenerationContext& context,
      const gd::String& previousParameter,
      std::vector<std::pair<gd::String, gd::String> >*
          supplementaryParametersTypes);

  virtual gd::String GenerateObjectFunctionCall(
      gd::String objectListName,
      const gd::ObjectMetadata& objMetadata,
      const gd::ExpressionCodeGenerationInformation& codeInfo,
      gd::String parametersStr,
      gd::String defaultOutput,
      gd::EventsCodeGenerationContext& context);

  virtual gd::String GenerateObjectBehaviorFunctionCall(
      gd::String objectListName,
      gd::String behaviorName,
      const gd::BehaviorMetadata& autoInfo,
      const gd::ExpressionCodeGenerationInformation& codeInfo,
      gd::String parametersStr,
      gd::String defaultOutput,
      gd::EventsCodeGenerationContext& context);

  virtual gd::String GenerateFreeCondition(
      const std::vector<gd::String>& arguments,
      const gd::InstructionMetadata& instrInfos,
      const gd::String& returnBoolean,
      bool conditionInverted,
      gd::EventsCodeGenerationContext& context);

  virtual gd::String GenerateObjectCondition(
      const gd::String& objectName,
      const gd::ObjectMetadata& objInfo,
      const std::vector<gd::String>& arguments,
      const gd::InstructionMetadata& instrInfos,
      const gd::String& returnBoolean,
      bool conditionInverted,
      gd::EventsCodeGenerationContext& context);

  virtual gd::String GenerateBehaviorCondition(
      const gd::String& objectName,
      const gd::String& behaviorName,
      const gd::BehaviorMetadata& autoInfo,
      const std::vector<gd::String>& arguments,
      const gd::InstructionMetadata& instrInfos,
      const gd::String& returnBoolean,
      bool conditionInverted,
      gd::EventsCodeGenerationContext& context);

  virtual gd::String GenerateObjectAction(
      const gd::String& objectName,
      const gd::ObjectMetadata& objInfo,
      const std::vector<gd::String>& arguments,
      const gd::InstructionMetadata& instrInfos,
      gd::EventsCodeGenerationContext& context);

  virtual gd::String GenerateBehaviorAction(
      const gd::String& objectName,
      const gd::String& behaviorName,
      const gd::BehaviorMetadata& autoInfo,
      const std::vector<gd::String>& arguments,
      const gd::InstructionMetadata& instrInfos,
      gd::EventsCodeGenerationContext& context);

  virtual gd::String GenerateNegatedPredicat(const gd::String& predicat) const {
    return "!(" + predicat + ")";
  };

  virtual gd::String GenerateReferenceToUpperScopeBoolean(
      const gd::String& referenceName,
      const gd::String& referencedBoolean,
      gd::EventsCodeGenerationContext& context);

  virtual gd::String GenerateObjectsDeclarationCode(
      gd::EventsCodeGenerationContext& context);

  virtual gd::String GenerateAllInstancesGetter(gd::String& objectName);

  virtual gd::String GenerateProfilerSectionBegin(const gd::String& section);
  virtual gd::String GenerateProfilerSectionEnd(const gd::String& section);

 private:
  /**
   * \brief Generate the declarations of all the booleans required to run
   * events.
   *
   * This should be called after generating events list code, so that the code
   * generator knows all the booleans to be declared.
   */
  gd::String GenerateAllConditionsBooleanDeclarations();

  /**
   * \brief Generate the declarations of all the objects list arrays.
   *
   * This should be called after generating events list code, with the maximum
   * depth reached by events.
   */
  std::pair<gd::String, gd::String> GenerateAllObjectsDeclarationsAndResets(
      unsigned int maxDepthLevelReached);

  /**
   * \brief Add to include files all the files required by the object and their
   * behaviors.
   */
  void AddAllObjectsIncludeFiles();

  gd::String GenerateEventsFunctionParameterDeclarationsList(const std::vector<gd::ParameterMetadata> & parameters);
  gd::String GenerateEventsFunctionContext(const std::vector<gd::ParameterMetadata> & parameters);

  /**
   * \brief Construct a code generator for the specified project and layout.
   */
  EventsCodeGenerator(gd::Project& project, const gd::Layout& layout);

  /**
   * \brief Construct a code generator for the specified objects and groups.
   */
  EventsCodeGenerator(gd::ClassWithObjects& globalObjectsAndGroups,
                      const gd::ClassWithObjects& objectsAndGroups);
  virtual ~EventsCodeGenerator();
};

}  // namespace gdjs
#endif  // EVENTSCODEGENERATOR_H
