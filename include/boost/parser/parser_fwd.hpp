#ifndef BOOST_PARSER_PARSER_FWD_HPP
#define BOOST_PARSER_PARSER_FWD_HPP

#include <boost/parser/error_handling_fwd.hpp>

#include <cstdint>


namespace boost { namespace parser {

    namespace detail {

        // TODO: -> boost::parser namespace
        /** A tag type used to represent the absence of information, value,
            etc., in `boost::parser`.  For instance, a parser with no global
            data will have a `globals_` data member whose type is `nope`. */
        struct nope;

        enum class flags : unsigned int {
            gen_attrs = 1 << 0,
            use_skip = 1 << 1,
            trace = 1 << 2,
            in_apply_parser = 1 << 3
        };

        // Built-in tags for use with the context.
        struct val_tag;
        struct attr_tag;
        struct where_tag;
        struct begin_tag;
        struct end_tag;
        struct pass_tag;
        struct locals_tag;
        struct rule_params_tag;
        struct globals_tag;
        struct trace_indent_tag;
        struct error_handler_tag;
        struct callbacks_tag;
        struct symbol_table_tries_tag;
    }

    /** Repeats the application of another parser of type `Parser p`,
        optionally applying another parser of type `DelimiterParser d` in
        between each pair of applications of `p`.  The parse succeeds if `p`
        succeeds at least the minumum number of times, and `d` succeeds each
        time it is applied.  The attribute produced is a sequence of the type
        of attribute produced by `Parser`. */
    template<
        typename Parser,
        typename DelimiterParser = detail::nope,
        typename MinType = int64_t,
        typename MaxType = int64_t>
    struct repeat_parser;

    /** Repeats the application of another parser of type `Parser [0, Inf)`
        times.  The parse always succeeds.  The attribute produced is a
        sequence of the type of attribute produced by `Parser`. */
    template<typename Parser>
    struct zero_plus_parser;

    /** Repeats the application of another parser of type `Parser p [1, Inf)`
        times.  The parse succeeds iff `p` succeeds at least once.  The
        attribute produced is a sequence of the type of attribute produced by
        `Parser`. */
    template<typename Parser>
    struct one_plus_parser;

    /** Repeats the application of another parser of type `Parser p [1, Inf)`
        times, applying a parser of type `DelimiterParser` in between each
        pair of applications of `p`.  The parse succeeds iff `p` succeeds at
        least once, and `d` succeeds each time it is applied.  The attribute
        produced is a sequence of the type of attribute produced by
        `Parser`. */
    template<typename Parser, typename DelimiterParser>
    struct delimited_seq_parser;

    /** Repeats the application of another parser of type `Parser [0, 1]`
        times.  The parse always succeeds.  The attribute produced is a
        `std::optional<T>`, where `T` is the type of attribute produced by
        `Parser`. */
    template<typename Parser>
    struct opt_parser;

    /** Applies each parser in `ParserTuple`, in order, stopping after the
        application of the first one that succeeds.  The parse succeeds iff
        one of the sub-parsers succeeds.  The attribute produced is a
        `std::variant` over the types of attribute produced by the parsers in
        `ParserTuple`. */
    template<typename ParserTuple>
    struct or_parser;

    /** Applies each parser in `ParserTuple`, in order.  The parse succeeds
        iff all of the sub-parsers succeeds.  The attribute produced is a
        `std::tuple` over the types of attribute produced by the parsers in
        `ParserTuple`.  The BacktrackingTuple template parameter is a
        `hana::tuple` of `hana::bool_` values.  The `i`th such value indicates
        whether backtracking is allowed if the `i`th parser fails. */
    template<typename ParserTuple, typename BacktrackingTuple>
    struct seq_parser;

    /** Applies the given parser of type `Parser p` and an invocable of type
        `Action a`.  `Action` shall model `semantic_action`, and `a` will only
        be invoked if `p` succeeds.  The parse succeeds iff `p` succeeds.
        Produces no attribute. */
    template<typename Parser, typename Action>
    struct action_parser;

    /** Applies the given parser of type `Parser p`; regardless of the
        attribute produced by `Parser`, this parser produces no attribute.
        The parse succeeds iff `p` succeeds. */
    template<typename Parser>
    struct omit_parser;

    /** Applies the given parser of type `Parser p`; regardless of the
        attribute produced by `Parser`, this parser's attribute is equivalent
        to `_where(ctx)` within a semantic action on `p`.  The parse succeeds
        iff `p` succeeds. */
    template<typename Parser>
    struct raw_parser;

    /** Applies the given parser of type `Parser p`, disabling the current
        skipper in use, if any.  The parse succeeds iff `p` succeeds.  The
        attribute produced is the type of attribute produced by `Parser`. */
    template<typename Parser>
    struct lexeme_parser;

    /** Applies the given parser of type `Parser p`, using a parser of type
        `SkipParser` as the skipper.  The parse succeeds iff `p` succeeds.
        The parse succeeds iff `p` succeeds.  The attribute produced is the
        type of attribute produced by `Parser`. */
    template<typename Parser, typename SkipParser = detail::nope>
    struct skip_parser;

    /** Applies the given parser of type `Parser p`, producing no attributes
        and consuming no input.  The parse succeeds iff `p` succeeds. */
    template<typename Parser, bool FailOnMatch>
    struct expect_parser;

    /** Matches one of a set S of possible inputs, each of which is associated
        with an attribute value of type `T`, forming a symbol table.  New
        elements and their associated attributes may be added to or removed
        from S dynamically, during parsing; any such changes are reverted at
        the end of parsing.  The parse succeeds iff an element of S is
        matched.  \see symbols */
    template<typename T>
    struct symbol_parser;

    /** TODO */
    template<
        bool UseCallbacks,
        typename Parser,
        typename Attribute,
        typename LocalState,
        typename ParamsTuple>
    struct rule_parser;

    template<typename T>
    struct ref_parser;

    /** Matches anything, and consumes no input.  If `Predicate` is anything
        other than `detail::nope`, and `predicate_(ctx)` evaluates to false,
        where `ctx` is the parser context, the parse fails. */
    template<typename Predicate>
    struct eps_parser;

    /** Matches only the end of input.  Produces no attribute. */
    struct eoi_parser;

    /** Matches anything, consumes no input, and produces an attribute of type
        `Attribute`. */
    template<typename Attribute>
    struct attr_parser;

    /** TODO */
    template<typename Expected, typename AttributeType = void>
    struct char_parser;

    /** Maches a particular string; produces no attribute. */
    struct string_parser;

    /** Maches an end-of-line in the input; produces no attribute.
        End-of-line is considered to be "\r\n", or any one of the line break
        code points from the Unicode Line Break Algorithm, described in
        https://unicode.org/reports/tr14. */
    struct eol_parser;

    /** Maches the strings "true" and "false", producing an attribute of
        `true` or `false`, respectively, and fails on any other input. */
    struct bool_parser;

    /** Matches an unsigned number of radix `Radix`, of at least `MinDigits`
        and at most `MaxDigits`, producing an attribute of type `T`.  Fails on
        any other input.  The parse will also fail if `Expected` is anything
        but `detail::nope`, and the produced attribute is not equal to
        `expected_`.  `Radix` must be one of 2, 8, 10, or 16. */
    template<
        typename T,
        int Radix = 10,
        int MinDigits = 1,
        int MaxDigits = -1,
        typename Expected = detail::nope>
    struct uint_parser;

    /** Matches a signed number of radix `Radix`, of at least `MinDigits` and
        at most `MaxDigits`, producing an attribute of type `T`.  Fails on any
        other input.  The parse will also fail if `Expected` is anything but
        `detail::nope`, and the produced attribute is not equal to
        `expected_`.  `Radix` must be one of 2, 8, 10, or 16. */
    template<
        typename T,
        int Radix = 10,
        int MinDigits = 1,
        int MaxDigits = -1,
        typename Expected = detail::nope>
    struct int_parser;

    /** Matches a floating point number, producing an attribute of type
        `T`. */
    template<typename T>
    struct float_parser;

    /** TODO */
    template<typename SwitchValue, typename OrParser = detail::nope>
    struct switch_parser;

    /** A wrapper for a parser, with optional global state and optional error
        handler, that provides the interface that all parsers must have:
        `operator*()`, `operator>>()`, etc. */
    template<
        typename Parser,
        typename GlobalState = detail::nope,
        typename ErrorHandler = default_error_handler>
    struct parser_interface;

}}

#endif
