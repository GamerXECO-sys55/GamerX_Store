math = cc.find_library('m', required: false)


libdex_opts = [
  'introspection=disabled',
]

libadwaita_dep       = dependency('libadwaita-1', version: '>= 1.7')
libdex_dep           = dependency('libdex-1', version: '>= 0.11.1', default_options: libdex_opts)
flatpak_dep          = dependency('flatpak', version: '>= 1.9')
appstream_dep        = dependency('appstream', version: '>= 1.0')
xmlb_dep             = dependency('xmlb', version: '>= 0.3.4')
yaml_dep             = dependency('yaml-0.1', version: '>= 0.2.5')
libsoup_dep          = dependency('libsoup-3.0', version: '>= 3.6.0')
json_glib_dep        = dependency('json-glib-1.0', version: '>= 1.10.0')
glycin_dep           = dependency('glycin-2', version: '>= 2.0')
glycin_gtk4_dep      = dependency('glycin-gtk4-2', version: '>= 2.0')


dl_worker_sources = [
  'gx-env.c',
  'gx-global-state.c',
  'dl-worker.c',
]

dl_worker_deps = [
  math,
  libdex_dep,
  libsoup_dep,
  json_glib_dep,
]

dl_worker_exe = executable(dl_worker_bin_name, dl_worker_sources,
  dependencies: dl_worker_deps,
       install: true,
)


marshalers = gnome.genmarshal('gx-marshalers',
  sources: 'gx-marshalers.list',
  prefix: 'gx_marshal',
  valist_marshallers: true,
)

gdbus_src = gnome.gdbus_codegen(
  'gs-shell-search-provider-generated',
  'shell-search-provider-dbus-interfaces.xml',
  interface_prefix : 'org.gnome.',
  namespace : 'Gx',
)

gx_sources = [
  'gx-addons-dialog.c',
  'gx-app-tile.c',
  'gx-application-map-factory.c',
  'gx-application.c',
  'gx-async-texture.c',
  'gx-backend-notification.c',
  'gx-backend-transaction-op-payload.c',
  'gx-backend-transaction-op-progress-payload.c',
  'gx-backend.c',
  'gx-browse-widget.c',
  'gx-category-page.c',
  'gx-category-tile.c',
  'gx-comet-overlay.c',
  'gx-comet.c',
  'gx-content-provider.c',
  'gx-content-section.c',
  'gx-curated-app-tile.c',
  'gx-data-graph.c',
  'gx-data-point.c',
  'gx-decorated-screenshot.c',
  'gx-detailed-app-tile.c',
  'gx-download-worker.c',
  'gx-dynamic-list-view.c',
  'gx-entry-cache-manager.c',
  'gx-entry-group.c',
  'gx-entry-inspector.c',
  'gx-entry.c',
  'gx-env.c',
  'gx-error.c',
  'gx-flathub-category.c',
  'gx-flathub-page.c',
  'gx-flathub-state.c',
  'gx-flatpak-entry.c',
  'gx-flatpak-instance.c',
  'gx-full-view.c',
  'gx-global-progress.c',
  'gx-global-state.c',
  'gx-gnome-shell-search-provider.c',
  'gx-group-tile-css-watcher.c',
  'gx-inhibited-scrollable.c',
  'gx-inspector.c',
  'gx-installed-page.c',
  'gx-io.c',
  'gx-issue.c',
  'gx-lazy-async-texture-model.c',
  'gx-patterned-background.c',
  'gx-preferences-dialog.c',
  'gx-progress-bar.c',
  'gx-release.c',
  'gx-result.c',
  'gx-review.c',
  'gx-screenshot.c',
  'gx-search-engine.c',
  'gx-search-result.c',
  'gx-search-widget.c',
  'gx-section-view.c',
  'gx-serializable.c',
  'gx-share-dialog.c',
  'gx-state-info.c',
  'gx-stats-dialog.c',
  'gx-transaction-manager.c',
  'gx-transaction-task.c',
  'gx-transaction-view.c',
  'gx-transaction.c',
  'gx-update-dialog.c',
  'gx-url.c',
  'gx-window.c',
  'gx-yaml-parser.c',
  'main.c',
]

gx_deps = [
  # bazaar_ui_dep,
  math,
  gtk_dep,
  libadwaita_dep,
  flatpak_dep,
  appstream_dep,
  xmlb_dep,
  yaml_dep,
  libsoup_dep,
  json_glib_dep,
  libdex_dep,
  glycin_dep,
  glycin_gtk4_dep,
]

blueprints = custom_target('blueprints',
  input: files(
    'gx-addons-dialog.blp',
    'gx-app-tile.blp',
    'gx-browse-widget.blp',
    'gx-category-tile.blp',
    'gx-category-page.blp',
    'gx-curated-app-tile.blp',
    'gx-decorated-screenshot.blp',
    'gx-detailed-app-tile.blp',
    'gx-entry-inspector.blp',
    'gx-flathub-page.blp',
    'gx-full-view.blp',
    'gx-inspector.blp',
    'gx-installed-page.blp',
    'gx-preferences-dialog.blp',
    'gx-progress-bar.blp',
    'gx-search-widget.blp',
    'gx-section-view.blp',
    'gx-share-dialog.blp',
    'gx-stats-dialog.blp',
    'gx-transaction-view.blp',
    'gx-update-dialog.blp',
    'gx-window.blp',
    'gtk/shortcuts-dialog.blp',
  ),
  output: '.',
  command: [
    find_program('blueprint-compiler', version: '>= 0.18.0'),
    'batch-compile',
    '@OUTPUT@',
    '@CURRENT_SOURCE_DIR@',
    '@INPUT@'
  ],
)

gx_sources += gnome.compile_resources('gx-resources',
  'gamerx-store.gresource.xml',
  c_name: 'gx',
  dependencies: blueprints
)

executable('gamerx-store', gx_sources, gdbus_src, marshalers,
           dependencies: gx_deps,
)
