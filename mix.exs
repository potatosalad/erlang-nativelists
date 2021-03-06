defmodule Nativelists.MixProject do
  use Mix.Project

  def project() do
    [
      app: :nativelists,
      version: "0.0.0",
      elixir: "~> 1.6",
      elixirc_paths: elixirc_paths(Mix.env()),
      build_embedded: Mix.env() == :prod,
      start_permanent: Mix.env() == :prod,
      deps: deps(),
      compilers: [:elixir_make] ++ Mix.compilers(),
      make_env: %{"MIX_ENV" => to_string(Mix.env())},
      make_clean: ["clean"],
      make_cwd: "c_src",
      description: description(),
      docs: fn ->
        {ref, 0} = System.cmd("git", ["rev-parse", "--verify", "--quiet", "HEAD"])
        [source_ref: ref, main: "Nativelists", extras: ["README.md", "CHANGELOG.md"]]
      end,
      name: "nativelists",
      package: package(),
      source_url: "https://github.com/potatosalad/erlang-nativelists"
    ]
  end

  # Run "mix help compile.app" to learn about applications.
  def application() do
    [
      extra_applications: [:logger]
    ]
  end

  # Run "mix help deps" to learn about dependencies.
  defp deps() do
    [
      {:elixir_make, "~> 0.4", runtime: false}
    ]
  end

  defp description() do
    """
    nativelists - experiments with native C list functions
    """
  end

  # Specifies which paths to compile per environment
  defp elixirc_paths(:test), do: ["lib", "test/support"]
  defp elixirc_paths(_), do: ["lib"]

  defp package() do
    [
      name: :nativelists,
      files: [
        "c_src",
        "CHANGELOG*",
        "include",
        "lib",
        "LICENSE*",
        "mix.exs",
        "priv",
        "README*",
        "rebar.config",
        "src"
      ],
      licenses: ["Mozilla Public License Version 2.0"],
      links: %{
        "GitHub" => "https://github.com/potatosalad/erlang-nativelists"
      },
      maintainers: ["Andrew Bennett"]
    ]
  end
end
